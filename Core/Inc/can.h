/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.h
  * @brief   This file contains all the function prototypes for
  *          the can.c file
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
#ifndef __CAN_H__
#define __CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern CAN_HandleTypeDef hcan1;

/* USER CODE BEGIN Private defines */

struct _CAN_ErrorFlags {
    uint8_t ewg : 1;           /*!< Protocol Error Warning                               */                     
    uint8_t epv : 1;           /*!< Error Passive                                        */                     
    uint8_t bof : 1;           /*!< Bus-off error                                        */                     
    uint8_t stf : 1;           /*!< Stuff error                                          */                     
    uint8_t form : 1;          /*!< Form error                                           */                      
    uint8_t ack : 1;           /*!< Acknowledgment error                                 */                     
    uint8_t br : 1;            /*!< Bit recessive error                                  */                    
    uint8_t bd : 1;            /*!< Bit dominant error                                   */                    
    uint8_t crc : 1;           /*!< CRC error                                            */                     
    uint8_t rx_fov0 : 1;       /*!< Rx FIFO0 overrun error                               */                         
    uint8_t rx_fov1 : 1;       /*!< Rx FIFO1 overrun error                               */                         
    uint8_t tx_alst0 : 1;      /*!< TxMailbox 0 transmit failure due to arbitration lost */                          
    uint8_t tx_terr0 : 1;      /*!< TxMailbox 0 transmit failure due to transmit error   */                         
    uint8_t tx_alst1 : 1;      /*!< TxMailbox 1 transmit failure due to arbitration lost */                          
    uint8_t tx_terr1 : 1;      /*!< TxMailbox 1 transmit failure due to transmit error   */                         
    uint8_t tx_alst2 : 1;      /*!< TxMailbox 2 transmit failure due to arbitration lost */                          
    uint8_t tx_terr2 : 1;      /*!< TxMailbox 2 transmit failure due to transmit error   */                         
    uint8_t timeout : 1;       /*!< Timeout error                                        */                         
    uint8_t not_initialized :1;/*!< Peripheral not initialized                           */
    uint8_t not_ready : 1;     /*!< Peripheral not ready                                 */                           
    uint8_t not_started : 1;   /*!< Peripheral not started                               */                             
    uint8_t param : 1;         /*!< Parameter error                                      */                       
    uint8_t inv_clbk : 1;      /*!< Invalid Callback error                               */
    uint8_t internal : 1;      /*!< Internal error                                       */                        
};

union CAN_ErrorFlags {
    struct _CAN_ErrorFlags error_flags_struct;
    uint32_t error_flags_u32;
};

/* USER CODE END Private defines */

void MX_CAN1_Init(void);

/* USER CODE BEGIN Prototypes */

HAL_StatusTypeDef CAN_send(CAN_HandleTypeDef *hcan, uint8_t *buffer, CAN_TxHeaderTypeDef *header);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */

