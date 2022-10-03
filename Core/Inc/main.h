/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

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
void Notify_Error(uint32_t *TxMailbox, uint8_t msg);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define curr_sens_Pin GPIO_PIN_2
#define curr_sens_GPIO_Port GPIOC
#define RELAY_CMD_Pin GPIO_PIN_4
#define RELAY_CMD_GPIO_Port GPIOA
#define ORANGE_LED_Pin GPIO_PIN_5
#define ORANGE_LED_GPIO_Port GPIOA
#define YELLOW_LED_Pin GPIO_PIN_10
#define YELLOW_LED_GPIO_Port GPIOB
#define gpio_RED_Pin GPIO_PIN_10
#define gpio_RED_GPIO_Port GPIOD
#define gpio_GREEN_Pin GPIO_PIN_12
#define gpio_GREEN_GPIO_Port GPIOD
#define gpio_BLUE_Pin GPIO_PIN_15
#define gpio_BLUE_GPIO_Port GPIOD
#define cmd_fan_Pin GPIO_PIN_10
#define cmd_fan_GPIO_Port GPIOC
#define Wakeup_Pin GPIO_PIN_6
#define Wakeup_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
