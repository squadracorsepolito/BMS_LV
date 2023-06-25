/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define M95256_NS_GPIO_OUT_Pin GPIO_PIN_0
#define M95256_NS_GPIO_OUT_GPIO_Port GPIOC
#define M95256_D_SPI_MOSI_Pin GPIO_PIN_1
#define M95256_D_SPI_MOSI_GPIO_Port GPIOC
#define CAS50NP_OUT_ADC_IN_Pin GPIO_PIN_2
#define CAS50NP_OUT_ADC_IN_GPIO_Port GPIOC
#define M95256_NW_GPIO_OUT_Pin GPIO_PIN_3
#define M95256_NW_GPIO_OUT_GPIO_Port GPIOC
#define NTC8_ADC2_IN_Pin GPIO_PIN_0
#define NTC8_ADC2_IN_GPIO_Port GPIOA
#define NTC9_ADC2_IN_Pin GPIO_PIN_1
#define NTC9_ADC2_IN_GPIO_Port GPIOA
#define NTC10_ADC2_IN_Pin GPIO_PIN_2
#define NTC10_ADC2_IN_GPIO_Port GPIOA
#define NTC11_ADC2_IN_Pin GPIO_PIN_3
#define NTC11_ADC2_IN_GPIO_Port GPIOA
#define NTC12_ADC2_IN_Pin GPIO_PIN_4
#define NTC12_ADC2_IN_GPIO_Port GPIOA
#define L9963T_SCK_SPI_SCK_Pin GPIO_PIN_5
#define L9963T_SCK_SPI_SCK_GPIO_Port GPIOA
#define L9963T_SDO_SPI_MISO_Pin GPIO_PIN_6
#define L9963T_SDO_SPI_MISO_GPIO_Port GPIOA
#define L9963T_SDI_SPI_MOSI_Pin GPIO_PIN_7
#define L9963T_SDI_SPI_MOSI_GPIO_Port GPIOA
#define L9963T_NCS_GPIO_OUT_Pin GPIO_PIN_4
#define L9963T_NCS_GPIO_OUT_GPIO_Port GPIOC
#define L9963T_DIS_GPIO_INOUT_Pin GPIO_PIN_5
#define L9963T_DIS_GPIO_INOUT_GPIO_Port GPIOC
#define L9963T_ISOFREQ_GPIO_OUT_Pin GPIO_PIN_0
#define L9963T_ISOFREQ_GPIO_OUT_GPIO_Port GPIOB
#define L9963T_BNE_GPIO_IN_Pin GPIO_PIN_1
#define L9963T_BNE_GPIO_IN_GPIO_Port GPIOB
#define L9963T_TXEN_GPIO_OUT_Pin GPIO_PIN_2
#define L9963T_TXEN_GPIO_OUT_GPIO_Port GPIOB
#define M95256_C_SPI_SCK_Pin GPIO_PIN_10
#define M95256_C_SPI_SCK_GPIO_Port GPIOB
#define M95256_Q_SPI_MISO_Pin GPIO_PIN_14
#define M95256_Q_SPI_MISO_GPIO_Port GPIOB
#define LED_ERR_GPIO_OUT_Pin GPIO_PIN_15
#define LED_ERR_GPIO_OUT_GPIO_Port GPIOB
#define LED_WARN_GPIO_OUT_Pin GPIO_PIN_6
#define LED_WARN_GPIO_OUT_GPIO_Port GPIOC
#define LED_STAT3_GPIO_OUT_Pin GPIO_PIN_7
#define LED_STAT3_GPIO_OUT_GPIO_Port GPIOC
#define LED_STAT2_GPIO_OUT_Pin GPIO_PIN_8
#define LED_STAT2_GPIO_OUT_GPIO_Port GPIOC
#define LED_STAT1_GPIO_OUT_Pin GPIO_PIN_9
#define LED_STAT1_GPIO_OUT_GPIO_Port GPIOC
#define T_VCP_TX_USART_TX_Pin GPIO_PIN_9
#define T_VCP_TX_USART_TX_GPIO_Port GPIOA
#define T_VCP_RX_USART_RX_Pin GPIO_PIN_10
#define T_VCP_RX_USART_RX_GPIO_Port GPIOA
#define SN65HVD23x_R_CAN_RX_Pin GPIO_PIN_11
#define SN65HVD23x_R_CAN_RX_GPIO_Port GPIOA
#define SN65HVD23x_D_CAN_TX_Pin GPIO_PIN_12
#define SN65HVD23x_D_CAN_TX_GPIO_Port GPIOA
#define LV_CMD_GPIO_OUT_Pin GPIO_PIN_5
#define LV_CMD_GPIO_OUT_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
