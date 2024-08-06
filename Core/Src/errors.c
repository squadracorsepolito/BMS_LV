#include "errors.h"
#include "tim.h"
#include "usart.h"
#include "logger_wrapper.h"
#include "can_utils.h"

void global_exp_cb(uint8_t error_index, uint8_t instance_index);
void global_toggle_cb(uint8_t error_index, uint8_t instance_index);
void undervoltage_exp_cb(uint8_t error_index, uint8_t instance_index);
void undervoltage_toggle_cb(uint8_t error_index, uint8_t instance_index);
void overvoltage_exp_cb(uint8_t error_index, uint8_t instance_index);
void overvoltage_toggle_cb(uint8_t error_index, uint8_t instance_index);
void overtemp_exp_cb(uint8_t error_index, uint8_t instance_index);
void overtemp_toggle_cb(uint8_t error_index, uint8_t instance_index);
void undertemp_exp_cb(uint8_t error_index, uint8_t instance_index);
void undertemp_toggle_cb(uint8_t error_index, uint8_t instance_index);

ERROR_UTILS_HandleTypeDef error_h;
ERROR_UTILS_ErrorInstanceTypeDef undervoltage_instances[7];
ERROR_UTILS_ErrorInstanceTypeDef overvoltage_instances[7];
ERROR_UTILS_ErrorInstanceTypeDef overtemp_instances[12];
ERROR_UTILS_ErrorInstanceTypeDef undertemp_instances[12];

ERROR_UTILS_ErrorTypeDef errors[] = {
    [ERRORS_UNDERVOLTAGE] = {.expiry_callback = undervoltage_exp_cb, .expiry_delay_ms = 500, .instances = undervoltage_instances, .instances_length = 7, .toggle_callback = undervoltage_toggle_cb},
    [ERRORS_OVERVOLTAGE] = {.expiry_callback = overvoltage_exp_cb, .expiry_delay_ms = 500, .instances = overvoltage_instances, .instances_length = 7, .toggle_callback = overvoltage_toggle_cb},
    [ERRORS_OVERTEMPERATURE] = {.expiry_callback = overtemp_exp_cb, .expiry_delay_ms = 1000, .instances = overtemp_instances, .instances_length = 12, .toggle_callback = overtemp_toggle_cb},
    [ERRORS_UNDERTEMPERATURE] = {.expiry_callback = undertemp_exp_cb, .expiry_delay_ms = 1000, .instances = undertemp_instances, .instances_length = 12, .toggle_callback = undertemp_toggle_cb},
};
ERROR_UTILS_ConfigTypeDef config = {.errors_array = errors, .errors_length = ERRORS_NUM};

void error_init(void)
{
    ERROR_UTILS_init(&error_h, &htim7, &config, global_toggle_cb, global_exp_cb);
}

void error_set_undervoltage(uint8_t index) {
    ERROR_UTILS_error_set(&error_h, ERRORS_UNDERVOLTAGE, index);
}

uint8_t error_get_undervoltage(uint8_t index) {
    return ERROR_UTILS_is_set(&error_h, ERRORS_UNDERVOLTAGE, index);
}

void error_set_overvoltage(uint8_t index) {
    ERROR_UTILS_error_set(&error_h, ERRORS_OVERVOLTAGE, index);
}

uint8_t error_get_overvoltage(uint8_t index) {
    return ERROR_UTILS_is_set(&error_h, ERRORS_OVERVOLTAGE, index);
}

void error_set_overtemp(uint8_t index) {
    ERROR_UTILS_error_set(&error_h, ERRORS_OVERTEMPERATURE, index);
}

uint8_t error_get_overtemp(uint8_t index) {
    return ERROR_UTILS_is_set(&error_h, ERRORS_OVERTEMPERATURE, index);
}

void error_set_undertemp(uint8_t index) {
    ERROR_UTILS_error_set(&error_h, ERRORS_UNDERTEMPERATURE, index);
}

uint8_t error_get_undertemp(uint8_t index) {
    return ERROR_UTILS_is_set(&error_h, ERRORS_UNDERTEMPERATURE, index);
}

void error_reset_undervoltage(uint8_t index) {
    ERROR_UTILS_error_reset(&error_h, ERRORS_UNDERVOLTAGE, index);
}

void error_reset_overvoltage(uint8_t index) {
    ERROR_UTILS_error_reset(&error_h, ERRORS_OVERVOLTAGE, index);
}

void error_reset_overtemp(uint8_t index) {
    ERROR_UTILS_error_reset(&error_h, ERRORS_OVERTEMPERATURE, index);
}

void error_reset_undertemp(uint8_t index) {
    ERROR_UTILS_error_reset(&error_h, ERRORS_UNDERTEMPERATURE, index);
}

void global_exp_cb(uint8_t error_index, uint8_t instance_index) {
    can_send_msg(MCB_BMSLV_STATUS_FRAME_ID);
    HAL_GPIO_WritePin(LV_CMD_GPIO_OUT_GPIO_Port, LV_CMD_GPIO_OUT_Pin, GPIO_PIN_RESET);
    logger_log(LOGGER_DEBUG, "global_exp, error_index: %d, instance: %d", error_index, instance_index);
}
void global_toggle_cb(uint8_t error_index, uint8_t instance_index) {
    logger_log(LOGGER_DEBUG, "global_toggle, error_index: %d, instance: %d", error_index, instance_index);
}
void undervoltage_exp_cb(uint8_t error_index, uint8_t instance_index) {
    logger_log(LOGGER_ERROR, "undervoltage on cell %d", instance_index);
}
void undervoltage_toggle_cb(uint8_t error_index, uint8_t instance_index) {
}
void overvoltage_exp_cb(uint8_t error_index, uint8_t instance_index) {
    logger_log(LOGGER_ERROR, "overvoltage on cell %d", instance_index);
}
void overvoltage_toggle_cb(uint8_t error_index, uint8_t instance_index) {
}
void overtemp_exp_cb(uint8_t error_index, uint8_t instance_index) {
    logger_log(LOGGER_ERROR, "overtemp on temp %d", instance_index);
}
void overtemp_toggle_cb(uint8_t error_index, uint8_t instance_index) {
}
void undertemp_exp_cb(uint8_t error_index, uint8_t instance_index) {
    logger_log(LOGGER_ERROR, "undertemp on temp %d", instance_index);
}
void undertemp_toggle_cb(uint8_t error_index, uint8_t instance_index) {
}


void error_timerElapsed_irq(TIM_HandleTypeDef *htim) {
    ERROR_UTILS_TimerElapsedCallback(&error_h, htim);
}