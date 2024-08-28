/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
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
#include "can.h"

#include "app_db.h"
#include "mcb.h"

/* USER CODE BEGIN 0 */

void CAN1_init(void);

/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;

/* CAN1 init function */
void MX_CAN1_Init(void) {
    /* USER CODE BEGIN CAN1_Init 0 */

    /* USER CODE END CAN1_Init 0 */

    /* USER CODE BEGIN CAN1_Init 1 */

    /* USER CODE END CAN1_Init 1 */
    hcan1.Instance                  = CAN1;
    hcan1.Init.Prescaler            = 3;
    hcan1.Init.Mode                 = CAN_MODE_NORMAL;
    hcan1.Init.SyncJumpWidth        = CAN_SJW_1TQ;
    hcan1.Init.TimeSeg1             = CAN_BS1_12TQ;
    hcan1.Init.TimeSeg2             = CAN_BS2_2TQ;
    hcan1.Init.TimeTriggeredMode    = DISABLE;
    hcan1.Init.AutoBusOff           = ENABLE;
    hcan1.Init.AutoWakeUp           = DISABLE;
    hcan1.Init.AutoRetransmission   = DISABLE;
    hcan1.Init.ReceiveFifoLocked    = DISABLE;
    hcan1.Init.TransmitFifoPriority = DISABLE;
    if (HAL_CAN_Init(&hcan1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN CAN1_Init 2 */
    CAN1_init();
    /* USER CODE END CAN1_Init 2 */
}

void HAL_CAN_MspInit(CAN_HandleTypeDef *canHandle) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (canHandle->Instance == CAN1) {
        /* USER CODE BEGIN CAN1_MspInit 0 */

        /* USER CODE END CAN1_MspInit 0 */
        /* CAN1 clock enable */
        __HAL_RCC_CAN1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
        GPIO_InitStruct.Pin       = SN65HVD23x_R_CAN_RX_Pin | SN65HVD23x_D_CAN_TX_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* CAN1 interrupt Init */
        HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
        HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
        /* USER CODE BEGIN CAN1_MspInit 1 */

        /* USER CODE END CAN1_MspInit 1 */
    }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef *canHandle) {
    if (canHandle->Instance == CAN1) {
        /* USER CODE BEGIN CAN1_MspDeInit 0 */

        /* USER CODE END CAN1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_CAN1_CLK_DISABLE();

        /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
        HAL_GPIO_DeInit(GPIOA, SN65HVD23x_R_CAN_RX_Pin | SN65HVD23x_D_CAN_TX_Pin);

        /* CAN1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
        HAL_NVIC_DisableIRQ(CAN1_SCE_IRQn);
        /* USER CODE BEGIN CAN1_MspDeInit 1 */

        /* USER CODE END CAN1_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

void CAN1_init(void) {
    CAN_FilterTypeDef filter;
    filter.FilterMode   = CAN_FILTERMODE_IDLIST;
    filter.FilterIdLow  = MCB_DIAG_TOOL_XCP_TX_BMS_LV_FRAME_ID << 5;  // Take all ids from 0
    filter.FilterIdHigh = MCB_DIAG_TOOL_XCP_TX_BMS_LV_FRAME_ID << 5;  // to 2^11 - 1
    /* HAL considers IdLow and IdHigh not as just the ID of the can message but
      as the combination of:
      STDID + RTR + IDE + 4 most significant bits of EXTID
  */
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterBank           = 0;
    filter.FilterScale          = CAN_FILTERSCALE_16BIT;
    filter.FilterActivation     = ENABLE;

    HAL_CAN_ConfigFilter(&hcan1, &filter);

    filter.FilterMode   = CAN_FILTERMODE_IDLIST;
    filter.FilterIdLow  = MCB_DASH_CAR_COMMANDS_FRAME_ID << 5;  // Take all ids from 0
    filter.FilterIdHigh = MCB_DASH_CAR_COMMANDS_FRAME_ID << 5;  // to 2^11 - 1
    /* HAL considers IdLow and IdHigh not as just the ID of the can message but
      as the combination of:
      STDID + RTR + IDE + 4 most significant bits of EXTID
  */
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterBank           = 1;
    filter.FilterScale          = CAN_FILTERSCALE_16BIT;
    filter.FilterActivation     = ENABLE;

    HAL_CAN_ConfigFilter(&hcan1, &filter);

    HAL_CAN_ActivateNotification(&hcan1,
                                 CAN_IT_ERROR | CAN_IT_BUSOFF | CAN_IT_ERROR_PASSIVE | CAN_IT_ERROR_WARNING |
                                     CAN_IT_RX_FIFO0_MSG_PENDING);
    HAL_CAN_Start(&hcan1);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef rx_header;
    uint8_t buffer[8] = {0};

    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, buffer) != HAL_OK) {
        return;
    }

    if (rx_header.StdId == MCB_DIAG_TOOL_XCP_TX_BMS_LV_FRAME_ID && buffer[0] == 0xff && buffer[1] == 0x00) {
        // Reset when preparing for flash via CAN bus
        NVIC_SystemReset();
    } else if (rx_header.StdId == MCB_DASH_CAR_COMMANDS_FRAME_ID && rx_header.DLC == MCB_DASH_CAR_COMMANDS_LENGTH) {
        struct mcb_dash_car_commands_t msg;
        mcb_dash_car_commands_unpack(&msg, buffer, MCB_DASH_CAR_COMMANDS_LENGTH);

        if (msg.bms_lv_diag_pwd == 0x420) {
            ADB.DIAG_mode_status = 1U;
            switch (msg.lv_relay_override_state) {
                case MCB_DASH_CAR_COMMANDS_LV_RELAY_OVERRIDE_STATE_LV_RLY_OVERRIDE_OFF_CHOICE:
                    ADB.LV_RLY_override_state = LV_RLY_override_OFF;
                    break;
                case MCB_DASH_CAR_COMMANDS_LV_RELAY_OVERRIDE_STATE_LV_RLY_OVERRIDE_OPEN_CHOICE:
                    ADB.LV_RLY_override_state = LV_RLY_override_OPEN;
                    break;
                case MCB_DASH_CAR_COMMANDS_LV_RELAY_OVERRIDE_STATE_LV_RLY_OVERRIDE_CLOSED_CHOICE:
                    ADB.LV_RLY_override_state = LV_RLY_override_CLOSED;
                    break;
                default:
                    break;
            }
        } else {
            ADB.DIAG_mode_status = 0U;
        }
    }
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) {
    static union CAN_ErrorFlags err = {};

    err.error_flags_u32 = HAL_CAN_GetError(hcan);

    uint16_t rec = (uint16_t)((hcan->Instance->ESR && CAN_ESR_REC_Msk) >> CAN_ESR_REC_Pos);
    uint16_t tec = (uint16_t)((hcan->Instance->ESR && CAN_ESR_TEC_Msk) >> CAN_ESR_TEC_Pos);
    //logger_log(LOGGER_RAW,
    //           "%u,%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;\r\n",
    //           hcan->State,
    //           err.error_flags_struct.ewg,
    //           err.error_flags_struct.epv,
    //           err.error_flags_struct.bof,
    //           err.error_flags_struct.stf,
    //           err.error_flags_struct.form,
    //           err.error_flags_struct.ack,
    //           err.error_flags_struct.br,
    //           err.error_flags_struct.bd,
    //           err.error_flags_struct.crc,
    //           err.error_flags_struct.rx_fov0,
    //           err.error_flags_struct.rx_fov1,
    //           err.error_flags_struct.tx_alst0,
    //           err.error_flags_struct.tx_terr0,
    //           err.error_flags_struct.tx_alst1,
    //           err.error_flags_struct.tx_terr1,
    //           err.error_flags_struct.tx_alst2,
    //           err.error_flags_struct.tx_terr2,
    //           err.error_flags_struct.timeout,
    //           err.error_flags_struct.not_initialized,
    //           err.error_flags_struct.not_ready,
    //           err.error_flags_struct.not_started,
    //           err.error_flags_struct.param,
    //           err.error_flags_struct.inv_clbk,
    //           err.error_flags_struct.internal,
    //           rec,tec
    //           );
    HAL_CAN_ResetError(hcan);
}

static HAL_StatusTypeDef CAN_wait(CAN_HandleTypeDef *hcan, uint8_t timeout) {
    uint32_t tick = HAL_GetTick();
    while (HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0) {
        if (HAL_GetTick() - tick > timeout)
            return HAL_TIMEOUT;
    }
    return HAL_OK;
}

HAL_StatusTypeDef CAN_send(CAN_HandleTypeDef *hcan, uint8_t *buffer, CAN_TxHeaderTypeDef *header) {
    if (CAN_wait(hcan, 1) != HAL_OK)
        return HAL_TIMEOUT;
    uint32_t mailbox;

    volatile HAL_StatusTypeDef status = HAL_CAN_AddTxMessage(hcan, header, buffer, &mailbox);

    return status;
}

/* USER CODE END 1 */
