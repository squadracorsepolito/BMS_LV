/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "eeprom-config.h"
#include "L9963E.h"
#include "stm32_if.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_CAN1_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
  uint32_t def = 11;
  uint32_t new = 22;
  HAL_GPIO_WritePin(M95256_NW_GPIO_OUT_GPIO_Port, M95256_NW_GPIO_OUT_Pin, GPIO_PIN_SET);
  EEPROM_ConfigTypeDef eeprom_config;
  EEPROM_config_init(&eeprom_config, &hspi2, M95256_NS_GPIO_OUT_GPIO_Port, M95256_NS_GPIO_OUT_Pin, 0x1, 1, (void*)&def, 4);


  L9963E_IfTypeDef interface = {
      .L9963E_IF_DelayMs = DelayMs,
      .L9963E_IF_GetTickMs = GetTickMs,
      .L9963E_IF_GPIO_ReadPin = GPIO_ReadPin,
      .L9963E_IF_GPIO_WritePin = GPIO_WritePin,
      .L9963E_IF_SPI_Receive = SPI_Receive,
      .L9963E_IF_SPI_Transmit = SPI_Transmit
    };

  HAL_UART_Transmit(&huart1, (uint8_t*)"test\r\n", 6, 10);

  L9963E_HandleTypeDef h9l;
  L9963E_init(&h9l, interface, 1);
  volatile L9963E_StatusTypeDef e = L9963E_addressing_procedure(&h9l, 0b11, 0, 0, 1);

  char *dioca = e == L9963E_OK ? "ok\r\n" : "ko\r\n";
  HAL_UART_Transmit(&huart1, (uint8_t*)dioca, 4, 10);

  L9963E_RegisterUnionTypeDef bal3_reg = {.generic = 0};
  L9963E_RegisterUnionTypeDef dev_gen_cfg_reg = {.generic = 0};

  L9963E_DRV_reg_read(&(h9l.drv_handle), 0x1, L9963E_DEV_GEN_CFG_ADDR, &dev_gen_cfg_reg, 5);
  e = L9963E_DRV_reg_read(&(h9l.drv_handle), 0x1, L9963E_Bal_3_ADDR, &bal3_reg, 5);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /*
    char out[10] = {0};
    HAL_GPIO_TogglePin(LED_STAT1_GPIO_OUT_GPIO_Port, LED_STAT1_GPIO_OUT_Pin);
    HAL_Delay(100);
    HAL_GPIO_TogglePin(LED_STAT2_GPIO_OUT_GPIO_Port, LED_STAT2_GPIO_OUT_Pin);
    HAL_Delay(100);
    HAL_GPIO_TogglePin(LED_STAT3_GPIO_OUT_GPIO_Port, LED_STAT3_GPIO_OUT_Pin);
    HAL_Delay(100);
    HAL_GPIO_TogglePin(LED_WARN_GPIO_OUT_GPIO_Port, LED_WARN_GPIO_OUT_Pin);
    HAL_Delay(100);
    HAL_GPIO_TogglePin(LED_ERR_GPIO_OUT_GPIO_Port, LED_ERR_GPIO_OUT_Pin);
    HAL_Delay(100);
    */

    L9963E_DRV_wakeup(&(h9l.drv_handle));
    /*
    sprintf(out, "%ld\r\n", *(uint32_t*)EEPROM_config_get(&eeprom_config));
    HAL_UART_Transmit(&huart1, (uint8_t*)out, strlen(out), 10);
    EEPROM_config_set(&eeprom_config, &new);
    EEPROM_config_write(&eeprom_config);
    */
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLRCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
