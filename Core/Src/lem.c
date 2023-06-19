#include "lem.h"

#include "tim.h"

#define LEM_EXP_SMOOTH_ALPHA 1

float current;

void lem_init(void) {
    current = 0;
    HAL_TIM_Base_Start(&htim2);
    HAL_ADC_Start_IT(&hadc1);
}

float lem_get_current(void) {
    return current;
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