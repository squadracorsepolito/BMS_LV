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
    filter.FilterMode       = CAN_FILTERMODE_IDLIST;
    filter.FilterIdLow      = 0x7 << 5;                 // Take all ids from 0
    filter.FilterIdHigh     = 0x7 << 5;  // to 2^11 - 1
    /* HAL considers IdLow and IdHigh not as just the ID of the can message but
        as the combination of: 
        STDID + RTR + IDE + 4 most significant bits of EXTID
    */
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterBank           = 0;
    filter.FilterScale          = CAN_FILTERSCALE_16BIT;
    filter.FilterActivation     = ENABLE;

    HAL_CAN_ConfigFilter(&hcan1, &filter);

    filter.FilterMode       = CAN_FILTERMODE_IDLIST;
    filter.FilterIdLow      = MCB_BMSLV_RELAY_OVERRIDE_FRAME_ID << 5;                 // Take all ids from 0
    filter.FilterIdHigh     = MCB_BMSLV_RELAY_OVERRIDE_FRAME_ID << 5;  // to 2^11 - 1
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
        struct mcb_bmslv_temp1_t bmslv_temp1;
        struct mcb_bmslv_temp2_t bmslv_temp2;
        struct mcb_bmslv_cell_voltage1_t bmslv_cell_voltage1;
        struct mcb_bmslv_cell_voltage2_t bmslv_cell_voltage2;
        struct mcb_bmslv_battery_pack_general_t bmslv_battery_pack_general;
        struct mcb_bmslv_status_t bmslv_status;
    } msgs;

    tx_header.StdId = id;

    switch (id)
    {
    case MCB_BMSLV_TEMP1_FRAME_ID:
        msgs.bmslv_temp2.ntc1_resistance = mcb_bmslv_temp2_ntc1_resistance_encode(ntc_get_ext_resistance(0));
        msgs.bmslv_temp2.ntc2_resistance = mcb_bmslv_temp2_ntc2_resistance_encode(ntc_get_ext_resistance(1));
        msgs.bmslv_temp2.ntc3_resistance = mcb_bmslv_temp2_ntc3_resistance_encode(ntc_get_ext_resistance(2));
        msgs.bmslv_temp2.ntc4_resistance = mcb_bmslv_temp2_ntc4_resistance_encode(ntc_get_ext_resistance(3));
        msgs.bmslv_temp2.ntc5_resistance = mcb_bmslv_temp2_ntc5_resistance_encode(ntc_get_ext_resistance(4));
        msgs.bmslv_temp2.ntc6_resistance = mcb_bmslv_temp2_ntc6_resistance_encode(ntc_get_ext_resistance(5));

        tx_header.DLC = mcb_bmslv_temp1_pack(buffer, &msgs.bmslv_temp1, MCB_BMSLV_TEMP1_LENGTH);
        can_send(&hcan1, buffer, &tx_header);
        break;
    case MCB_BMSLV_TEMP2_FRAME_ID:
        msgs.bmslv_temp2.ntc1_resistance = mcb_bmslv_temp2_ntc1_resistance_encode(ntc_get_ext_resistance(6));
        msgs.bmslv_temp2.ntc2_resistance = mcb_bmslv_temp2_ntc2_resistance_encode(ntc_get_int_resistance(0));
        msgs.bmslv_temp2.ntc3_resistance = mcb_bmslv_temp2_ntc3_resistance_encode(ntc_get_int_resistance(1));
        msgs.bmslv_temp2.ntc4_resistance = mcb_bmslv_temp2_ntc4_resistance_encode(ntc_get_int_resistance(2));
        msgs.bmslv_temp2.ntc5_resistance = mcb_bmslv_temp2_ntc5_resistance_encode(ntc_get_int_resistance(3));
        msgs.bmslv_temp2.ntc6_resistance = mcb_bmslv_temp2_ntc6_resistance_encode(ntc_get_int_resistance(4));

        tx_header.DLC = mcb_bmslv_temp2_pack(buffer, &msgs.bmslv_temp2, MCB_BMSLV_TEMP2_LENGTH);
        can_send(&hcan1, buffer, &tx_header);
        break;
    case MCB_BMSLV_CELL_VOLTAGE1_FRAME_ID:
        msgs.bmslv_cell_voltage1.cell_1_voltage_m_v = mcb_bmslv_cell_voltage1_cell_1_voltage_m_v_encode(L9963E_utils_get_cell_mv(0));
        msgs.bmslv_cell_voltage1.cell_2_voltage_m_v = mcb_bmslv_cell_voltage1_cell_2_voltage_m_v_encode(L9963E_utils_get_cell_mv(1));
        msgs.bmslv_cell_voltage1.cell_3_voltage_m_v = mcb_bmslv_cell_voltage1_cell_3_voltage_m_v_encode(L9963E_utils_get_cell_mv(2));
        msgs.bmslv_cell_voltage1.cell_4_voltage_m_v = mcb_bmslv_cell_voltage1_cell_4_voltage_m_v_encode(L9963E_utils_get_cell_mv(3));

        tx_header.DLC = mcb_bmslv_cell_voltage1_pack(buffer, &msgs.bmslv_cell_voltage1, MCB_BMSLV_CELL_VOLTAGE1_LENGTH);
        can_send(&hcan1, buffer, &tx_header);
        break;
    case MCB_BMSLV_CELL_VOLTAGE2_FRAME_ID:
        msgs.bmslv_cell_voltage2.cell_5_voltage_m_v = mcb_bmslv_cell_voltage1_cell_1_voltage_m_v_encode(L9963E_utils_get_cell_mv(4));
        msgs.bmslv_cell_voltage2.cell_6_voltage_m_v = mcb_bmslv_cell_voltage1_cell_2_voltage_m_v_encode(L9963E_utils_get_cell_mv(5));
        msgs.bmslv_cell_voltage2.cell_7_voltage_m_v = mcb_bmslv_cell_voltage1_cell_3_voltage_m_v_encode(L9963E_utils_get_cell_mv(6));

        tx_header.DLC = mcb_bmslv_cell_voltage2_pack(buffer, &msgs.bmslv_cell_voltage2, MCB_BMSLV_CELL_VOLTAGE2_LENGTH);
        can_send(&hcan1, buffer, &tx_header);
        break;
    case MCB_BMSLV_BATTERY_PACK_GENERAL_FRAME_ID:
        float vtot, vsumbatt;
        L9963E_utils_get_batt_mv(&vtot, &vsumbatt);
        msgs.bmslv_battery_pack_general.current_sensor_m_v = mcb_bmslv_battery_pack_general_current_sensor_m_v_encode(lem_get_current_mv());
        msgs.bmslv_battery_pack_general.lv_total_voltage_m_v = mcb_bmslv_battery_pack_general_lv_total_voltage_m_v_encode(vtot);
        msgs.bmslv_battery_pack_general.lv_total_voltage_sum_m_v = mcb_bmslv_battery_pack_general_lv_total_voltage_m_v_encode(vsumbatt);

        tx_header.DLC = mcb_bmslv_battery_pack_general_pack(buffer, &msgs.bmslv_battery_pack_general, MCB_BMSLV_BATTERY_PACK_GENERAL_LENGTH);
        can_send(&hcan1, buffer, &tx_header);
        break;
    case MCB_BMSLV_STATUS_FRAME_ID:
        msgs.bmslv_status.is_relay_open = HAL_GPIO_ReadPin(LV_CMD_GPIO_OUT_GPIO_Port, LV_CMD_GPIO_OUT_Pin);
        msgs.bmslv_status.bmslv_status_message = 0xBB;

        tx_header.DLC = mcb_bmslv_status_pack(buffer, &msgs.bmslv_status, MCB_BMSLV_STATUS_LENGTH);
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

    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, buffer) != HAL_OK) {
        return;
    }

    if(rx_header.StdId == 0x007 && rx_header.DLC == 2 && buffer[0] == 0xff && buffer[1] == 0x00) {
        NVIC_SystemReset();
    } else if(rx_header.StdId == MCB_BMSLV_RELAY_OVERRIDE_FRAME_ID && rx_header.DLC == MCB_BMSLV_RELAY_OVERRIDE_LENGTH) {
        HAL_GPIO_WritePin(LV_CMD_GPIO_OUT_GPIO_Port, LV_CMD_GPIO_OUT_Pin, GPIO_PIN_SET);
    }
}