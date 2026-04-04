#include "main.h"
#include "FreeRTOS.h"
#include "timers.h"
/**
 * @brief 计算 Hamming(7,4) 编码后输出缓冲区所需的字节数
 *
 * 每字节含 2 个 nibble，每个 nibble 编码为 7 bit，
 * 所以 num_bytes 字节的编码输出位数为 num_bytes * 14，
 * 向上取整到字节边界。
 */
#define HAMMING74_OUTPUT_SIZE(num_bytes) (((uint32_t)(num_bytes) * 14u + 7u) / 8u)

void Antenna_Phase_Flip(void);
uint16_t Hamming74_Encode(const uint8_t *input, uint16_t num_bytes,
                          uint8_t *output);
uint16_t Antenna_Switch_Sequence_Generation(uint8_t bytes[], uint16_t num_bytes,
                                            uint8_t codebook_length,
                                            uint8_t preamble_length,
                                            uint8_t **seq);