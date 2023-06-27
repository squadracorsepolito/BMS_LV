#include "can_utils.h"

#include "ntc.h"
#include "L9963E_utils.h"
#include "lem.h"

#include "logger_wrapper.h"

CAN_TxHeaderTypeDef tx_header;

void can_tx_header_init() {
    tx_header.ExtId = 0;
    tx_header.IDE   = CAN_ID_STD;
    tx_header.RTR   = CAN_RTR_DATA;
}

void can_init(void) {
    CAN_FilterTypeDef filter;
    filter.FilterMode       = CAN_FILTERMODE_IDMASK;
    filter.FilterIdLow      = 0x7 << 5;                 // Take all ids from 0
    filter.FilterIdHigh     = 0x7 << 5;  // to 2^11 - 1
    filter.FilterMaskIdHigh = 0x7 << 5;                 // Don't care on can id bits
    filter.FilterMaskIdLow  = 0x7 << 5;                 // Don't care on can id bits
    /* HAL considers IdLow and IdHigh not as just the ID of the can message but
        as the combination of: 
        STDID + RTR + IDE + 4 most significant bits of EXTID
    */
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterBank           = 0;
    filter.FilterScale          = CAN_FILTERSCALE_16BIT;
    filter.FilterActivation     = ENABLE;

    HAL_CAN_ConfigFilter(&hcan1, &filter);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_ERROR | CAN_IT_RX_FIFO0_MSG_PENDING);
    HAL_CAN_Start(&hcan1);
}

HAL_StatusTypeDef can_wait(CAN_HandleTypeDef *hcan, uint8_t timeout) {
    uint32_t tick = HAL_GetTick();
    while (HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0) {
        if(HAL_GetTick() - tick > timeout) return HAL_TIMEOUT;
    }
    return HAL_OK;
}

HAL_StatusTypeDef can_send(CAN_HandleTypeDef *hcan, uint8_t *buffer, CAN_TxHeaderTypeDef *header) {
    if(can_wait(hcan, 1) != HAL_OK) return HAL_TIMEOUT;
    uint32_t mailbox;

    volatile HAL_StatusTypeDef status = HAL_CAN_AddTxMessage(hcan, header, buffer, &mailbox);

    return status;
}

void can_send_msg(uint32_t id) {
    uint8_t buffer[8] = {0};
    union {
        struct sc22_evo_canlv_bmslv_temp_t bmslv_temp;
        struct sc22_evo_canlv_bmslv_cell_voltage1_t bmslv_cell_voltage1;
        struct sc22_evo_canlv_bmslv_cell_voltage2_t bmslv_cell_voltage2;
        struct sc22_evo_canlv_bmslv_battery_pack_general_t bmslv_battery_pack_general;
        struct sc22_evo_canlv_bmslv_status_t bmslv_status;
    } msgs;

    tx_header.StdId = id;

    switch (id)
    {
    case SC22_EVO_CANLV_BMSLV_TEMP_FRAME_ID:
        msgs.bmslv_temp.ntc_is_first_message = 1;
        msgs.bmslv_temp.ntc1_resistance = sc22_evo_canlv_bmslv_temp_ntc1_resistance_encode(ntc_get_ext_resistance(0));
        msgs.bmslv_temp.ntc2_resistance = sc22_evo_canlv_bmslv_temp_ntc2_resistance_encode(ntc_get_ext_resistance(1));
        msgs.bmslv_temp.ntc3_resistance = sc22_evo_canlv_bmslv_temp_ntc3_resistance_encode(ntc_get_ext_resistance(2));
        msgs.bmslv_temp.ntc4_resistance = sc22_evo_canlv_bmslv_temp_ntc4_resistance_encode(ntc_get_ext_resistance(3));
        msgs.bmslv_temp.ntc5_resistance = sc22_evo_canlv_bmslv_temp_ntc5_resistance_encode(ntc_get_ext_resistance(4));
        msgs.bmslv_temp.ntc6_resistance = sc22_evo_canlv_bmslv_temp_ntc6_resistance_encode(ntc_get_ext_resistance(5));

        tx_header.DLC = sc22_evo_canlv_bmslv_temp_pack(buffer, &msgs.bmslv_temp, SC22_EVO_CANLV_BMSLV_TEMP_LENGTH);
        can_send(&hcan1, buffer, &tx_header);

        msgs.bmslv_temp.ntc_is_first_message = 0;
        msgs.bmslv_temp.ntc1_resistance = sc22_evo_canlv_bmslv_temp_ntc1_resistance_encode(ntc_get_ext_resistance(6));
        msgs.bmslv_temp.ntc2_resistance = sc22_evo_canlv_bmslv_temp_ntc2_resistance_encode(ntc_get_int_resistance(0));
        msgs.bmslv_temp.ntc3_resistance = sc22_evo_canlv_bmslv_temp_ntc3_resistance_encode(ntc_get_int_resistance(1));
        msgs.bmslv_temp.ntc4_resistance = sc22_evo_canlv_bmslv_temp_ntc4_resistance_encode(ntc_get_int_resistance(2));
        msgs.bmslv_temp.ntc5_resistance = sc22_evo_canlv_bmslv_temp_ntc5_resistance_encode(ntc_get_int_resistance(3));
        msgs.bmslv_temp.ntc6_resistance = sc22_evo_canlv_bmslv_temp_ntc6_resistance_encode(ntc_get_int_resistance(4));

        tx_header.DLC = sc22_evo_canlv_bmslv_temp_pack(buffer, &msgs.bmslv_temp, SC22_EVO_CANLV_BMSLV_TEMP_LENGTH);
        can_send(&hcan1, buffer, &tx_header);
        break;
    case SC22_EVO_CANLV_BMSLV_CELL_VOLTAGE1_FRAME_ID:
        msgs.bmslv_cell_voltage1.cell_1_voltage_m_v = sc22_evo_canlv_bmslv_cell_voltage1_cell_1_voltage_m_v_encode(L9963E_utils_get_cell_v(0));
        msgs.bmslv_cell_voltage1.cell_2_voltage_m_v = sc22_evo_canlv_bmslv_cell_voltage1_cell_2_voltage_m_v_encode(L9963E_utils_get_cell_v(1));
        msgs.bmslv_cell_voltage1.cell_3_voltage_m_v = sc22_evo_canlv_bmslv_cell_voltage1_cell_3_voltage_m_v_encode(L9963E_utils_get_cell_v(2));
        msgs.bmslv_cell_voltage1.cell_4_voltage_m_v = sc22_evo_canlv_bmslv_cell_voltage1_cell_4_voltage_m_v_encode(L9963E_utils_get_cell_v(3));

        tx_header.DLC = sc22_evo_canlv_bmslv_cell_voltage1_pack(buffer, &msgs.bmslv_cell_voltage1, SC22_EVO_CANLV_BMSLV_CELL_VOLTAGE1_LENGTH);
        can_send(&hcan1, buffer, &tx_header);
        break;
    case SC22_EVO_CANLV_BMSLV_CELL_VOLTAGE2_FRAME_ID:
        msgs.bmslv_cell_voltage2.cell_5_voltage_m_v = sc22_evo_canlv_bmslv_cell_voltage1_cell_1_voltage_m_v_encode(L9963E_utils_get_cell_v(4));
        msgs.bmslv_cell_voltage2.cell_6_voltage_m_v = sc22_evo_canlv_bmslv_cell_voltage1_cell_2_voltage_m_v_encode(L9963E_utils_get_cell_v(5));
        msgs.bmslv_cell_voltage2.cell_7_voltage_m_v = sc22_evo_canlv_bmslv_cell_voltage1_cell_3_voltage_m_v_encode(L9963E_utils_get_cell_v(6));

        tx_header.DLC = sc22_evo_canlv_bmslv_cell_voltage2_pack(buffer, &msgs.bmslv_cell_voltage2, SC22_EVO_CANLV_BMSLV_CELL_VOLTAGE2_LENGTH);
        can_send(&hcan1, buffer, &tx_header);
        break;
    case SC22_EVO_CANLV_BMSLV_BATTERY_PACK_GENERAL_FRAME_ID:
        msgs.bmslv_battery_pack_general.current_sensor_voltage = sc22_evo_canlv_bmslv_battery_pack_general_current_sensor_voltage_encode(lem_get_current_ampere());
        msgs.bmslv_battery_pack_general.lv_total_voltage_m_v = 0xdead;

        tx_header.DLC = sc22_evo_canlv_bmslv_battery_pack_general_pack(buffer, &msgs.bmslv_battery_pack_general, SC22_EVO_CANLV_BMSLV_BATTERY_PACK_GENERAL_LENGTH);
        can_send(&hcan1, buffer, &tx_header);
        break;
    case SC22_EVO_CANLV_BMSLV_STATUS_FRAME_ID:
        msgs.bmslv_status.is_relay_open = HAL_GPIO_ReadPin(LV_CMD_GPIO_OUT_GPIO_Port, LV_CMD_GPIO_OUT_Pin);
        msgs.bmslv_status.bmslv_status_message = 0xBB;

        tx_header.DLC = sc22_evo_canlv_bmslv_status_pack(buffer, &msgs.bmslv_status, SC22_EVO_CANLV_BMSLV_STATUS_LENGTH);
        can_send(&hcan1, buffer, &tx_header);
        break;
    default:
        break;
    }
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) {
    logger_log(LOGGER_ERROR, "sborato");
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef rx_header;
    uint8_t buffer[8] = {0};

    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, buffer) == HAL_OK) {
        if(rx_header.StdId == 0x007) {
            NVIC_SystemReset();
        }
    }
}