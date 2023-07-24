#include "lem.h"

#include "tim.h"

#define LEM_EXP_SMOOTH_ALPHA 0.1

float zero_offset = 2.5;
float current;

void lem_init(void) {
    current = 0;
    HAL_ADC_Start_IT(&hadc1);
}

float lem_get_current(void) {
    return current - (2.5 - zero_offset);
}

void lem_register_zero_offset(void) {
    zero_offset = current * 3.3 / 4095;
}

float lem_get_current_ampere(void) {
    return ((current * 3.3 / 4095) - zero_offset) / 0.0125;
}

float lem_get_current_mv(void) {
    return current * 3300 / 4095;
}

void lem_new_value(uint16_t val) {
    static uint8_t is_new = 1;

    if(is_new) {
        current = val;
        is_new = 0;
    }

    current = LEM_EXP_SMOOTH_ALPHA*val + (1-LEM_EXP_SMOOTH_ALPHA)*current;
}

void lem_adc_callback(ADC_HandleTypeDef *hadc) {
    lem_new_value(hadc->Instance->DR);
}