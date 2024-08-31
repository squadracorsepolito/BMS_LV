#include "can_utils.h"

#include "L9963E_utils.h"
#include "app_db.h"
#include "can.h"
#include "conf.h"
#include "errors.h"
#include "lem.h"
#include "logger_wrapper.h"
#include "ntc.h"

void can_tx_header_init() {
}

void MCB_send_msg(uint32_t id) {
    CAN_TxHeaderTypeDef tx_header;
    uint8_t buffer[8] = {0};

    float vtot, vsumbatt;

    union {
        struct mcb_bms_lv_hello_t hello;
        struct mcb_bms_lv_lv_cell_ntc_resistance0_t lv_cell_ntc_resistance0;
        struct mcb_bms_lv_lv_cell_ntc_resistance1_t lv_cell_ntc_resistance1;
        struct mcb_bms_lv_lv_cell_voltage0_t lv_cell_voltage0;
        struct mcb_bms_lv_lv_cell_voltage1_t lv_cell_voltage1;
        struct mcb_bms_lv_lv_bat_general_t lv_bat_general;
        struct mcb_bms_lv_cells_status_t cells_status;
        struct mcb_bms_lv_status_t status;
    } msg = {};

    tx_header.ExtId = 0;
    tx_header.IDE   = CAN_ID_STD;
    tx_header.RTR   = CAN_RTR_DATA;

    tx_header.StdId = id;

    // clang-format off
    switch (id) {
        case MCB_BMS_LV_HELLO_FRAME_ID:
            msg.hello.fw_major_version = mcb_bms_lv_hello_fw_major_version_encode(VERSION_MAJOR);
            msg.hello.fw_minor_version = mcb_bms_lv_hello_fw_minor_version_encode(VERSION_MINOR);
            msg.hello.fw_patch_version = mcb_bms_lv_hello_fw_patch_version_encode(VERSION_PATCH);

            tx_header.DLC = mcb_bms_lv_hello_pack(buffer, &msg.hello, 8U);
            break;
        case MCB_BMS_LV_LV_CELL_NTC_RESISTANCE0_FRAME_ID:
            msg.lv_cell_ntc_resistance0.lv_cell_ntc_00_resistance = mcb_bms_lv_lv_cell_ntc_resistance0_lv_cell_ntc_00_resistance_encode(ntc_get_ext_temp(0)*100);
            msg.lv_cell_ntc_resistance0.lv_cell_ntc_01_resistance = mcb_bms_lv_lv_cell_ntc_resistance0_lv_cell_ntc_01_resistance_encode(ntc_get_ext_temp(1)*100);
            msg.lv_cell_ntc_resistance0.lv_cell_ntc_02_resistance = mcb_bms_lv_lv_cell_ntc_resistance0_lv_cell_ntc_02_resistance_encode(ntc_get_ext_temp(2)*100);
            msg.lv_cell_ntc_resistance0.lv_cell_ntc_03_resistance = mcb_bms_lv_lv_cell_ntc_resistance0_lv_cell_ntc_03_resistance_encode(ntc_get_ext_temp(3)*100);
            msg.lv_cell_ntc_resistance0.lv_cell_ntc_04_resistance = mcb_bms_lv_lv_cell_ntc_resistance0_lv_cell_ntc_04_resistance_encode(ntc_get_ext_temp(4)*100);
            msg.lv_cell_ntc_resistance0.lv_cell_ntc_05_resistance = mcb_bms_lv_lv_cell_ntc_resistance0_lv_cell_ntc_05_resistance_encode(ntc_get_ext_temp(5)*100);

            tx_header.DLC = mcb_bms_lv_lv_cell_ntc_resistance0_pack(buffer, &msg.lv_cell_ntc_resistance0, 8U);
            break;
        case MCB_BMS_LV_LV_CELL_NTC_RESISTANCE1_FRAME_ID:
            msg.lv_cell_ntc_resistance1.lv_cell_ntc_06_resistance = mcb_bms_lv_lv_cell_ntc_resistance1_lv_cell_ntc_06_resistance_encode(ntc_get_ext_temp(6)*100);
            msg.lv_cell_ntc_resistance1.lv_cell_ntc_07_resistance = mcb_bms_lv_lv_cell_ntc_resistance1_lv_cell_ntc_07_resistance_encode(ntc_get_int_temp(0)*100);
            msg.lv_cell_ntc_resistance1.lv_cell_ntc_08_resistance = mcb_bms_lv_lv_cell_ntc_resistance1_lv_cell_ntc_08_resistance_encode(ntc_get_int_temp(1)*100);
            msg.lv_cell_ntc_resistance1.lv_cell_ntc_09_resistance = mcb_bms_lv_lv_cell_ntc_resistance1_lv_cell_ntc_09_resistance_encode(ntc_get_int_temp(2)*100);
            msg.lv_cell_ntc_resistance1.lv_cell_ntc_10_resistance = mcb_bms_lv_lv_cell_ntc_resistance1_lv_cell_ntc_10_resistance_encode(ntc_get_int_temp(3)*100);
            msg.lv_cell_ntc_resistance1.lv_cell_ntc_11_resistance = mcb_bms_lv_lv_cell_ntc_resistance1_lv_cell_ntc_11_resistance_encode(ntc_get_int_temp(4)*100);

            tx_header.DLC = mcb_bms_lv_lv_cell_ntc_resistance1_pack(buffer, &msg.lv_cell_ntc_resistance1, 8U);
            break;
        case MCB_BMS_LV_LV_CELL_VOLTAGE0_FRAME_ID:
            msg.lv_cell_voltage0.lv_cell_0_voltage = mcb_bms_lv_lv_cell_voltage0_lv_cell_0_voltage_encode(L9963E_utils_get_cell_mv(0));
            msg.lv_cell_voltage0.lv_cell_1_voltage = mcb_bms_lv_lv_cell_voltage0_lv_cell_1_voltage_encode(L9963E_utils_get_cell_mv(1));
            msg.lv_cell_voltage0.lv_cell_2_voltage = mcb_bms_lv_lv_cell_voltage0_lv_cell_2_voltage_encode(L9963E_utils_get_cell_mv(2));
            msg.lv_cell_voltage0.lv_cell_3_voltage = mcb_bms_lv_lv_cell_voltage0_lv_cell_3_voltage_encode(L9963E_utils_get_cell_mv(3));

            tx_header.DLC =
                mcb_bms_lv_lv_cell_voltage0_pack(buffer, &msg.lv_cell_voltage0, 8U);
            break;
        case MCB_BMS_LV_LV_CELL_VOLTAGE1_FRAME_ID:
            msg.lv_cell_voltage1.lv_cell_4_voltage = mcb_bms_lv_lv_cell_voltage1_lv_cell_4_voltage_encode(L9963E_utils_get_cell_mv(4));
            msg.lv_cell_voltage1.lv_cell_5_voltage = mcb_bms_lv_lv_cell_voltage1_lv_cell_5_voltage_encode(L9963E_utils_get_cell_mv(5));
            msg.lv_cell_voltage1.lv_cell_6_voltage = mcb_bms_lv_lv_cell_voltage1_lv_cell_6_voltage_encode(L9963E_utils_get_cell_mv(6));

            tx_header.DLC =
                mcb_bms_lv_lv_cell_voltage1_pack(buffer, &msg.lv_cell_voltage1, 8U);
            break;
        case MCB_BMS_LV_LV_BAT_GENERAL_FRAME_ID:
            L9963E_utils_get_batt_mv(&vtot, &vsumbatt);
            msg.lv_bat_general.lv_bat_current_sens_voltage = mcb_bms_lv_lv_bat_general_lv_bat_current_sens_voltage_encode(lem_get_current_mv());
            msg.lv_bat_general.lv_bat_voltage = mcb_bms_lv_lv_bat_general_lv_bat_voltage_encode(vtot);
            msg.lv_bat_general.lv_bat_summed_voltage = mcb_bms_lv_lv_bat_general_lv_bat_summed_voltage_encode(vsumbatt);

            tx_header.DLC = mcb_bms_lv_lv_bat_general_pack(
                buffer, &msg.lv_bat_general, 8U);
            break;
        case MCB_BMS_LV_CELLS_STATUS_FRAME_ID:
            msg.cells_status.lv_cell_0_is_ov = error_get_overvoltage(0);
            msg.cells_status.lv_cell_1_is_ov = error_get_overvoltage(1);
            msg.cells_status.lv_cell_2_is_ov = error_get_overvoltage(2);
            msg.cells_status.lv_cell_3_is_ov = error_get_overvoltage(3);
            msg.cells_status.lv_cell_4_is_ov = error_get_overvoltage(4);
            msg.cells_status.lv_cell_5_is_ov = error_get_overvoltage(5);
            msg.cells_status.lv_cell_6_is_ov = error_get_overvoltage(6);

            msg.cells_status.lv_cell_0_is_uv = error_get_undervoltage(0);
            msg.cells_status.lv_cell_1_is_uv = error_get_undervoltage(1);
            msg.cells_status.lv_cell_2_is_uv = error_get_undervoltage(2);
            msg.cells_status.lv_cell_3_is_uv = error_get_undervoltage(3);
            msg.cells_status.lv_cell_4_is_uv = error_get_undervoltage(4);
            msg.cells_status.lv_cell_5_is_uv = error_get_undervoltage(5);
            msg.cells_status.lv_cell_6_is_uv = error_get_undervoltage(6);

            msg.cells_status.lv_cell_ntc_00_is_ot = error_get_overtemp(0);
            msg.cells_status.lv_cell_ntc_01_is_ot = error_get_overtemp(1);
            msg.cells_status.lv_cell_ntc_02_is_ot = error_get_overtemp(2);
            msg.cells_status.lv_cell_ntc_03_is_ot = error_get_overtemp(3);
            msg.cells_status.lv_cell_ntc_04_is_ot = error_get_overtemp(4);
            msg.cells_status.lv_cell_ntc_05_is_ot = error_get_overtemp(5);
            msg.cells_status.lv_cell_ntc_06_is_ot = error_get_overtemp(6);
            msg.cells_status.lv_cell_ntc_07_is_ot = error_get_overtemp(7);
            msg.cells_status.lv_cell_ntc_08_is_ot = error_get_overtemp(8);
            msg.cells_status.lv_cell_ntc_09_is_ot = error_get_overtemp(9);
            msg.cells_status.lv_cell_ntc_10_is_ot = error_get_overtemp(10);
            msg.cells_status.lv_cell_ntc_11_is_ot = error_get_overtemp(11);

            tx_header.DLC = mcb_bms_lv_cells_status_pack(buffer, &msg.cells_status, 8U);
            break;
        // case MCB_BMS_LV_STATUS_FRAME_ID:
        //     msg.status.lv_relay_is_closed = HAL_GPIO_ReadPin(LV_CMD_GPIO_OUT_GPIO_Port, LV_CMD_GPIO_OUT_Pin);
        //     msg.status.lv_relay_is_relay_override_on = ADB.LV_RLY_override_state != LV_RLY_override_OFF ? 1U : 0U ;

        //     tx_header.DLC = mcb_bms_lv_status_pack(buffer, &msg.status, 8U);
        default:
            break;
    }
    // clang-format on

    CAN_send(&hcan1, buffer, &tx_header);
}
