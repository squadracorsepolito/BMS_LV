#include "can_utils.h"

#include "ntc.h"
#include "L9963E_utils.h"
#include "lem.h"
#include "errors.h"

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
    filter.FilterBank           = 1;
    filter.FilterScale          = CAN_FILTERSCALE_16BIT;
    filter.FilterActivation     = ENABLE;

    HAL_CAN_ConfigFilter(&hcan1, &filter);

    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_ERROR | CAN_IT_RX_FIFO0_MSG_PENDING);
    HAL_CAN_Start(&hcan1);
    can_send_msg(MCB_BMSLV_HELO_FRAME_ID);
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
        struct mcb_bmslv_helo_t bmslv_helo;
        struct mcb_bmslv_temp1_t bmslv_temp1;
        struct mcb_bmslv_temp2_t bmslv_temp2;
        struct mcb_bmslv_cell_voltage1_t bmslv_cell_voltage1;
        struct mcb_bmslv_cell_voltage2_t bmslv_cell_voltage2;
        struct mcb_bmslv_battery_pack_general_t bmslv_battery_pack_general;
        struct mcb_bmslv_status_t bmslv_status;
    } msgs;
    float vtot, vsumbatt;

    tx_header.StdId = id;

    switch (id)
    {
    case MCB_BMSLV_HELO_FRAME_ID:
        msgs.bmslv_helo.time = mcb_bmslv_helo_time_encode(*(uint64_t*)__TIME__);

        tx_header.DLC = mcb_bmslv_helo_pack(buffer, &msgs.bmslv_helo, MCB_BMSLV_HELO_LENGTH);
        break;
    case MCB_BMSLV_TEMP1_FRAME_ID:
        msgs.bmslv_temp1.ntc1_resistance = mcb_bmslv_temp1_ntc1_resistance_encode(ntc_get_ext_temp(0)*1000);
        msgs.bmslv_temp1.ntc2_resistance = mcb_bmslv_temp1_ntc2_resistance_encode(ntc_get_ext_temp(1)*1000);
        msgs.bmslv_temp1.ntc3_resistance = mcb_bmslv_temp1_ntc3_resistance_encode(ntc_get_ext_temp(2)*1000);
        msgs.bmslv_temp1.ntc4_resistance = mcb_bmslv_temp1_ntc4_resistance_encode(ntc_get_ext_temp(3)*1000);
        msgs.bmslv_temp1.ntc5_resistance = mcb_bmslv_temp1_ntc5_resistance_encode(ntc_get_ext_temp(4)*1000);
        msgs.bmslv_temp1.ntc6_resistance = mcb_bmslv_temp1_ntc6_resistance_encode(ntc_get_ext_temp(5)*1000);

        tx_header.DLC = mcb_bmslv_temp1_pack(buffer, &msgs.bmslv_temp1, MCB_BMSLV_TEMP1_LENGTH);
        break;
    case MCB_BMSLV_TEMP2_FRAME_ID:
        msgs.bmslv_temp2.ntc1_resistance = mcb_bmslv_temp2_ntc1_resistance_encode(ntc_get_ext_temp(6)*1000);
        msgs.bmslv_temp2.ntc2_resistance = mcb_bmslv_temp2_ntc2_resistance_encode(ntc_get_int_temp(0)*1000);
        msgs.bmslv_temp2.ntc3_resistance = mcb_bmslv_temp2_ntc3_resistance_encode(ntc_get_int_temp(1)*1000);
        msgs.bmslv_temp2.ntc4_resistance = mcb_bmslv_temp2_ntc4_resistance_encode(ntc_get_int_temp(2)*1000);
        msgs.bmslv_temp2.ntc5_resistance = mcb_bmslv_temp2_ntc5_resistance_encode(ntc_get_int_temp(3)*1000);
        msgs.bmslv_temp2.ntc6_resistance = mcb_bmslv_temp2_ntc6_resistance_encode(ntc_get_int_temp(4)*1000);

        tx_header.DLC = mcb_bmslv_temp2_pack(buffer, &msgs.bmslv_temp2, MCB_BMSLV_TEMP2_LENGTH);
        break;
    case MCB_BMSLV_CELL_VOLTAGE1_FRAME_ID:
        msgs.bmslv_cell_voltage1.cell_1_voltage_m_v = mcb_bmslv_cell_voltage1_cell_1_voltage_m_v_encode(L9963E_utils_get_cell_mv(0));
        msgs.bmslv_cell_voltage1.cell_2_voltage_m_v = mcb_bmslv_cell_voltage1_cell_2_voltage_m_v_encode(L9963E_utils_get_cell_mv(1));
        msgs.bmslv_cell_voltage1.cell_3_voltage_m_v = mcb_bmslv_cell_voltage1_cell_3_voltage_m_v_encode(L9963E_utils_get_cell_mv(2));
        msgs.bmslv_cell_voltage1.cell_4_voltage_m_v = mcb_bmslv_cell_voltage1_cell_4_voltage_m_v_encode(L9963E_utils_get_cell_mv(3));

        tx_header.DLC = mcb_bmslv_cell_voltage1_pack(buffer, &msgs.bmslv_cell_voltage1, MCB_BMSLV_CELL_VOLTAGE1_LENGTH);
        break;
    case MCB_BMSLV_CELL_VOLTAGE2_FRAME_ID:
        msgs.bmslv_cell_voltage2.cell_5_voltage_m_v = mcb_bmslv_cell_voltage1_cell_1_voltage_m_v_encode(L9963E_utils_get_cell_mv(4));
        msgs.bmslv_cell_voltage2.cell_6_voltage_m_v = mcb_bmslv_cell_voltage1_cell_2_voltage_m_v_encode(L9963E_utils_get_cell_mv(5));
        msgs.bmslv_cell_voltage2.cell_7_voltage_m_v = mcb_bmslv_cell_voltage1_cell_3_voltage_m_v_encode(L9963E_utils_get_cell_mv(6));

        tx_header.DLC = mcb_bmslv_cell_voltage2_pack(buffer, &msgs.bmslv_cell_voltage2, MCB_BMSLV_CELL_VOLTAGE2_LENGTH);
        break;
    case MCB_BMSLV_BATTERY_PACK_GENERAL_FRAME_ID:
        L9963E_utils_get_batt_mv(&vtot, &vsumbatt);
        msgs.bmslv_battery_pack_general.current_sensor_m_v = mcb_bmslv_battery_pack_general_current_sensor_m_v_encode(lem_get_current_mv());
        msgs.bmslv_battery_pack_general.lv_total_voltage_m_v = mcb_bmslv_battery_pack_general_lv_total_voltage_m_v_encode(vtot);
        msgs.bmslv_battery_pack_general.lv_total_voltage_sum_m_v = mcb_bmslv_battery_pack_general_lv_total_voltage_m_v_encode(vsumbatt);

        tx_header.DLC = mcb_bmslv_battery_pack_general_pack(buffer, &msgs.bmslv_battery_pack_general, MCB_BMSLV_BATTERY_PACK_GENERAL_LENGTH);
        break;
    case MCB_BMSLV_STATUS_FRAME_ID:
        msgs.bmslv_status.is_relay_open = !HAL_GPIO_ReadPin(LV_CMD_GPIO_OUT_GPIO_Port, LV_CMD_GPIO_OUT_Pin);
        msgs.bmslv_status.cell1_ov = error_get_overvoltage(0);
        msgs.bmslv_status.cell2_ov = error_get_overvoltage(1);
        msgs.bmslv_status.cell3_ov = error_get_overvoltage(2);
        msgs.bmslv_status.cell4_ov = error_get_overvoltage(3);
        msgs.bmslv_status.cell5_ov = error_get_overvoltage(4);
        msgs.bmslv_status.cell6_ov = error_get_overvoltage(5);
        msgs.bmslv_status.cell7_ov = error_get_overvoltage(6);

        msgs.bmslv_status.cell1_uv = error_get_undervoltage(0);
        msgs.bmslv_status.cell2_uv = error_get_undervoltage(1);
        msgs.bmslv_status.cell3_uv = error_get_undervoltage(2);
        msgs.bmslv_status.cell4_uv = error_get_undervoltage(3);
        msgs.bmslv_status.cell5_uv = error_get_undervoltage(4);
        msgs.bmslv_status.cell6_uv = error_get_undervoltage(5);
        msgs.bmslv_status.cell7_uv = error_get_undervoltage(6);

        msgs.bmslv_status.temp1_ot = error_get_overtemp(0);
        msgs.bmslv_status.temp2_ot = error_get_overtemp(1);
        msgs.bmslv_status.temp3_ot = error_get_overtemp(2);
        msgs.bmslv_status.temp4_ot = error_get_overtemp(3);
        msgs.bmslv_status.temp5_ot = error_get_overtemp(4);
        msgs.bmslv_status.temp6_ot = error_get_overtemp(5);
        msgs.bmslv_status.temp7_ot = error_get_overtemp(6);
        msgs.bmslv_status.temp8_ot = error_get_overtemp(7);
        msgs.bmslv_status.temp9_ot = error_get_overtemp(8);
        msgs.bmslv_status.temp10_ot = error_get_overtemp(9);
        msgs.bmslv_status.temp11_ot = error_get_overtemp(10);
        msgs.bmslv_status.temp12_ot = error_get_overtemp(11);

        tx_header.DLC = mcb_bmslv_status_pack(buffer, &msgs.bmslv_status, MCB_BMSLV_STATUS_LENGTH);
        break;
    default:
        return;
    }
    can_send(&hcan1, buffer, &tx_header);
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) {
    // logger_log(LOGGER_ERROR, "sborato");
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
        struct mcb_bmslv_relay_override_t msg;
        mcb_bmslv_relay_override_unpack(&msg, buffer, MCB_BMSLV_RELAY_OVERRIDE_LENGTH);

        if(msg.password == 0x420)
            HAL_GPIO_WritePin(LV_CMD_GPIO_OUT_GPIO_Port, LV_CMD_GPIO_OUT_Pin, msg.close_relay ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}
