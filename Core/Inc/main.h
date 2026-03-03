/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern TIM_HandleTypeDef htim4;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void radio_thread_entry(void *parameter);
void keyboard_thread_entry(void *parameter);
void disp_thread_entry(void *parameter);
void Delayus_Using_TIM(uint32_t us);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define I2C_SCL_Pin GPIO_PIN_0
#define I2C_SCL_GPIO_Port GPIOC
#define I2C_SDA_Pin GPIO_PIN_1
#define I2C_SDA_GPIO_Port GPIOC
#define RESET_Pin GPIO_PIN_0
#define RESET_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define KEY_DOWN_Pin GPIO_PIN_10
#define KEY_DOWN_GPIO_Port GPIOB
#define DIO7_Pin GPIO_PIN_7
#define DIO7_GPIO_Port GPIOC
#define KEY_CONFIRM_Pin GPIO_PIN_8
#define KEY_CONFIRM_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define BUSY_Pin GPIO_PIN_3
#define BUSY_GPIO_Port GPIOB
#define KEY_UP_Pin GPIO_PIN_4
#define KEY_UP_GPIO_Port GPIOB
#define KEY_MENU_Pin GPIO_PIN_5
#define KEY_MENU_GPIO_Port GPIOB
#define CS_Pin GPIO_PIN_6
#define CS_GPIO_Port GPIOB
#define DIO8_Pin GPIO_PIN_8
#define DIO8_GPIO_Port GPIOB
#define DIO9_Pin GPIO_PIN_9
#define DIO9_GPIO_Port GPIOB
#define DIO9_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
