#include "errors.h"
#include "tim.h"
#include "usart.h"
#include "logger_wrapper.h"

void global_exp_cb(uint8_t error_index, uint8_t instance_index);
void global_toggle_cb(uint8_t error_index, uint8_t instance_index);
void undervoltage_exp_cb(uint8_t error_index, uint8_t instance_index);
void undervoltage_toggle_cb(uint8_t error_index, uint8_t instance_index);

ERROR_UTILS_HandleTypeDef error_h;
ERROR_UTILS_ErrorInstanceTypeDef undervoltage_instances[1];
ERROR_UTILS_ErrorTypeDef errors[] = {
    [ERRORS_UNDERVOLTAGE] = {.expiry_callback = undervoltage_exp_cb, .expiry_delay_ms = 500, .instances = undervoltage_instances, .instances_length = 1, .toggle_callback = undervoltage_toggle_cb}};
ERROR_UTILS_ConfigTypeDef config = {.errors_array = errors, .errors_length = 1};

void error_init(void)
{
    ERROR_UTILS_init(&error_h, &htim7, &config, global_toggle_cb, global_exp_cb);
}

void error_set_undervoltage(uint8_t index) {
    ERROR_UTILS_error_set(&error_h, ERRORS_UNDERVOLTAGE, index);
}

void global_exp_cb(uint8_t error_index, uint8_t instance_index) {
    logger_log(LOGGER_DEBUG, "global_exp");
}
void global_toggle_cb(uint8_t error_index, uint8_t instance_index) {
    logger_log(LOGGER_DEBUG, "global_toggle");
}
void undervoltage_exp_cb(uint8_t error_index, uint8_t instance_index) {
    logger_log(LOGGER_DEBUG, "under_exp");
}
void undervoltage_toggle_cb(uint8_t error_index, uint8_t instance_index) {
    logger_log(LOGGER_DEBUG, "under_toggle");
}