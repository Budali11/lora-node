#ifndef _MODULE_REDIRECT_H_
#define _MODULE_REDIRECT_H_
#include "main.h"
#include "cmsis_os.h"
#include <stdint.h>

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t
#define USE_IIC 1
#define SOFT_IIC 1
#define IIC_ADR 0x3C
#define IIC_CMD	0X00
#define IIC_RAM	0X40

enum BUTTON_ID{
    BUTTON_MENU_ID = 0,
    BUTTON_UP_ID,
    BUTTON_DOWN_ID,
    BUTTON_CONFIRM_ID
};

#define KEY_MENU_ID 0
#define KEY_UP_ID 1
#define KEY_DOWN_ID 2
#define KEY_CONFIRM_ID 3

extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart2;
#if !SOFT_IIC
extern I2C_HandleTypeDef hi2c3;
#endif

#define DEV_Digital_Write(port, pin, value) \
    HAL_GPIO_WritePin(port, pin, value)

#define DEV_Digital_Read(port, pin) \
    HAL_GPIO_ReadPin(port, pin)

#define DEV_SPI_Write_Bytes(tx_buf, length) \
    HAL_SPI_Transmit(&hspi1, tx_buf, length, HAL_MAX_DELAY)

#define DEV_SPI_Read_Bytes(rx_buf, length) \
    HAL_SPI_Receive(&hspi1, rx_buf, length, 0xffff)

#define DEV_Delay_ms(ms) \
    HAL_Delay(ms)

#define Driver_Delay_ms(ms) \
    HAL_Delay(ms)

#define I2C_W_SDA(value) \
    HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, value)

#define I2C_W_SCL(value) \
    HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, value)

#if defined(SOFT_IIC)
#define I2C_Start()   \
    do {                \
        I2C_W_SDA(1);   \
        I2C_W_SCL(1);   \
        vTaskDelay(2);    \
        I2C_W_SDA(0);   \
    } while (0)

#define I2C_Stop()    \
    do {                \
        I2C_W_SDA(0);   \
        I2C_W_SCL(1);   \
        vTaskDelay(2);    \
        I2C_W_SDA(1);   \
    } while (0)

#define I2C_Write_Byte(data)    \
    do {                     \
        for (int i = 0; i < 8; i++) { \
            vTaskDelay(2);    \
            I2C_W_SCL(0);   \
            vTaskDelay(2);    \
            if (((data) << i) & 0x80) { \
                I2C_W_SDA(1); \
            } else {         \
                I2C_W_SDA(0); \
            }                   \
            vTaskDelay(2);        \
            I2C_W_SCL(1);   \
        }                    \
        I2C_W_SCL(0);       \
    } while (0)

#define I2C_Write_Command(cmd) \
    do {                     \
        I2C_Start();        \
        I2C_Write_Byte(IIC_ADR << 1); \
        I2C_Write_Byte(IIC_CMD); \
        I2C_Write_Byte(cmd); \
        I2C_Stop();         \
    } while (0)

#define I2C_Write_nData(data, cnt)   \
    do {                     \
        I2C_Start();        \
        I2C_Write_Byte(IIC_ADR << 1); \
        I2C_Write_Byte(IIC_RAM); \
        for (int i = 0; i < cnt; i++) { \
            I2C_Write_Byte((data)[i]); \
        }                    \
        I2C_Stop();         \
    } while (0)
#define I2C_Write_Data(data)   \
    do {                     \
        I2C_Start();        \
        I2C_Write_Byte(IIC_ADR << 1); \
        I2C_Write_Byte(IIC_RAM); \
        I2C_Write_Byte(data); \
        I2C_Stop();         \
    } while (0)
#else
#define I2C_Write_Byte(data, mode) \
    do { \
        uint8_t buf[2]; \
        buf[0] = mode; \
        buf[1] = data; \
        HAL_I2C_Master_Transmit(&hi2c3, IIC_ADR << 1, buf, 2, HAL_MAX_DELAY); \
    } while(0)
#endif

#define Debug(...) \
    do { \
        /*rt_kprintf(__VA_ARGS__); */ \
    } while(0)


#define GPIO_WriteBit(GPIOx, GPIO_Pin, BitVal) \
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, (BitVal) ? GPIO_PIN_SET : GPIO_PIN_RESET)

uint8_t Button_Read(uint8_t button_id);
#endif