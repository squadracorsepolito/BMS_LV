#ifndef __BMS_LV_H__
#define __BMS_LV_H__

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "main.h"

// CAN ID
#define PING_ID_Tx 0x030
#define REBOOT_ID_CAN 0x150
#define CURRENT_SENSOR 0x051
#define BMS_LV_CELL_1 0x052
#define BMS_LV_CELL_2 0x053
#define BMS_LV_TEMP_1 0x054
#define BMS_LV_TEMP_2 0x055
#define GENERAL_ERROR 0x56
#define OPEN_RELAY 0x057

#define BETA 3400.0
#define T0 298 //Kelvin = 25 °C

// SYSTEM FAILURE - 0x00 - 0x0F
#define ERROR_OPEN_RELAY 0x0F

// CAN - 0x10 - 0x1F
#define ERROR_CAN_INIT 0x38 
#define ERROR_CAN_RX 0x39

// UART - 0x20 - 0x2F
#define ERROR_UART 0x3A
#define ERROR_UART_RX 0x40
#define ERROR_UART_BAUD_RATE 0x3B
#define ERROR_UART_ACTIVE_CH 0x3C
#define ERROR_UART_CELL_UV_TH 0x3D
#define ERROR_UART_CELL_OV_TH 0x3E
#define ERROR_UART_AUX_OV_TH 0x3F


//Ping ID Rx(to vehicle)
//#define PING_ID_Tx 0x030

//Ping ID Rx(to vehicle)
#define PING_ID_Rx 0x00  //NOT USED 

/* VARIABLES */
extern ADC_HandleTypeDef hadc2;
extern DMA_HandleTypeDef hdma_adc2;

extern CAN_HandleTypeDef hcan;
extern CAN_FilterTypeDef sFilterConfig;

extern TIM_HandleTypeDef htim3;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern uint32_t TxMailbox;
/* PROTOTYPES */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
uint32_t ReturnTime_100us(void);
int delay_fun (uint32_t *delay_100us_last, uint32_t delay_100us);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);

void CAN_Msg_Send(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *pHeader, uint8_t aData[], uint32_t *pTxMailbox, uint32_t TimeOut);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void CAN_Tx(uint32_t delay_100us);

void LedBlinking(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t delay_100us);

HAL_StatusTypeDef Uart_Send_CMD(int timeout, int size, ...);
HAL_StatusTypeDef Uart_Receive_CMD(uint8_t *cmd, int size, int timeout);

unsigned short crc16(unsigned short crc, unsigned char const *buffer, unsigned int len);

void GetVoltagesandTemperature(uint32_t delay_100us);
void SetupBmsLV(void);
void CoreBmsLV(uint32_t delay_100us);
void Ping_Mng(uint32_t delay_100us);

void Open_Relay(uint8_t msg);

#endif
