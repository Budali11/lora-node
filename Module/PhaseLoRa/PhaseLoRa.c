#include "PhaseLoRa.h"
#include "main.h"
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_tim.h"
#include <stdlib.h>

// BusOut ant_sw(PC_11, PD_2);
// Ticker fliper;
// extern TimerHandle_t ant_flipper_timer;

extern uint8_t *ant_sw_seq;
extern uint16_t seq_len;
extern uint16_t seq_idx;
void Antenna_Phase_Flip(void) {
    if (ant_sw_seq[seq_idx] == 1) {
        // ant_sw.write(1);
        GPIOC->BSRR = (GPIO_PIN_10 << 16) | GPIO_PIN_12;
    }
    else {
        GPIOC->BSRR = (GPIO_PIN_12 << 16) | GPIO_PIN_10;
    }
    if (seq_idx++ == seq_len) {
        HAL_TIM_Base_Stop_IT(&htim4);
        __HAL_TIM_SET_COUNTER(&htim4, 0);
        __HAL_TIM_CLEAR_IT(&htim4, TIM_IT_UPDATE);
        __HAL_TIM_CLEAR_FLAG(&htim4, TIM_FLAG_UPDATE);
        free(ant_sw_seq);
        ant_sw_seq = NULL;
        seq_idx = 0;
    }
}

uint16_t Antenna_Switch_Sequence_Generation(uint8_t bytes[], uint16_t num_bytes,
                                            uint8_t codebook_length,
                                            uint8_t preamble_length,
                                            uint8_t **seq) {
  if (num_bytes == 0 || codebook_length < 2 || seq == NULL) {
    return 0;
  }

  // 1. 计算 valid_bits = log2(codebook_length)
  uint8_t valid_bits = 0;
  uint8_t temp_cb = codebook_length;
  while (temp_cb >>= 1) {
    valid_bits++;
  }

  // 2. 计算需要提取多少个索引元素
  // 总比特数 / valid_bits，向上取整
  uint32_t total_bits = (uint32_t)num_bytes * 8;
  uint16_t num_indices = (uint16_t)((total_bits + valid_bits - 1) / valid_bits);

  // 3. 构建 temp 数组并提取比特
  // 使用 calloc 确保初始化为 0，方便后补 0 的逻辑
  uint8_t *temp = (uint8_t *)calloc(num_indices, sizeof(uint8_t));
  if (!temp)
    return 0;

  for (uint32_t i = 0; i < total_bits; i++) {
    // 当前比特属于第几个 temp 元素
    uint16_t temp_idx = i / valid_bits;

    // 从 bytes 中提取对应的比特 (MSB First)
    uint8_t byte_val = bytes[i / 8];
    uint8_t bit = (byte_val >> (7 - (i % 8))) & 1;

    // 将比特压入 temp[temp_idx]
    // 如果是该元素的最后一个比特，或者 total_bits 结束了，
    // 这种左移方式会自动让不足 valid_bits 的元素在高位补0。
    temp[temp_idx] = (temp[temp_idx] << 1) | bit;
  }

  // 4. 计算 seq 总长度并分配内存
  // seq 长度 = 索引个数 * 每个码本的长度
  uint32_t total_seq_len = (uint32_t)num_indices * codebook_length +
                           (preamble_length + 1 + 3 + 8) * codebook_length;
  // 1 for SFD, 3 for num_segments, 8 for num_payload

  // 注意：如果 total_seq_len 超过 65535，uint16_t 会溢出
  *seq = (uint8_t *)malloc(total_seq_len * sizeof(uint8_t));
  if (!(*seq)) {
    free(temp);
    return 0;
  }
  uint8_t *payload_part =
      (*seq + ((preamble_length + 1 + 3 + 8) * codebook_length));

  /* fill other parts of the packet */
  // preamble
  for (uint8_t i = 0; i < preamble_length; i++) {
    for (uint8_t j = 0; j < codebook_length; j++) {
      (*seq)[i * codebook_length + j] = (j < (codebook_length >> 1));
    }
  }
  // SFD
  uint8_t *sfd_part = (*seq) + (preamble_length * codebook_length);
  for (uint8_t i = 0; i < codebook_length; i++) {
    sfd_part[i] = 0;
  }
  // number of segments
  uint8_t *segments_part = sfd_part + codebook_length;
  uint8_t num_segments = codebook_length;
  uint8_t temp_num_seg = num_segments;
  uint8_t e_num_seg = 0;
  while (temp_num_seg >>= 1)
    e_num_seg++;
  for (uint8_t i = 0; i < 3; i++) {
    if ((e_num_seg & (1 << (2 - i))) != 0) {
      for (uint8_t j = 0; j < codebook_length; j++) {
        segments_part[i * codebook_length + j] = (j < (codebook_length >> 1));
      }
    } else {
      for (uint8_t j = 0; j < codebook_length; j++) {
        segments_part[i * codebook_length + j] = 0;
      }
    }
  }
  // payload length part
  uint8_t *payload_len_part = segments_part + 3 * codebook_length;
  for (uint8_t i = 0; i < 8; i++) {
    if ((num_indices & (1 << (7 - i))) != 0) {
      for (uint8_t j = 0; j < codebook_length; j++) {
        payload_len_part[i * codebook_length + j] =
            (j < (codebook_length >> 1));
      }
    } else {
      for (uint8_t j = 0; j < codebook_length; j++) {
        payload_len_part[i * codebook_length + j] = 0;
      }
    }
  }

  // 5. 根据 temp 中的索引映射正交码 (Walsh-Hadamard 逻辑)
  for (uint16_t i = 0; i < num_indices; i++) {
    uint8_t row_idx = temp[i]; // 码本的行索引

    for (uint8_t col_idx = 0; col_idx < codebook_length; col_idx++) {
      /*
       * 动态生成 Walsh 码：
       * 第 i 行第 j 列的值 = (i & j) 的二进制中 1 的个数的奇偶性
       */
      uint8_t combined = row_idx & col_idx;
      uint8_t count = 0;
      while (combined) {
        count ^= (combined & 1);
        combined >>= 1;
      }

      // 存入输出序列 (0 或 1)
      (payload_part)[i * codebook_length + col_idx] = count;
    }
  }

  free(temp);
  return (uint16_t)total_seq_len;
}