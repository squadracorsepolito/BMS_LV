#include "lem.h"

#include "tim.h"

#define LEM_EXP_SMOOTH_ALPHA 0.1f

uint16_t current;

void lem_init(void) {
    current = 0;
    HAL_TIM_Base_Start(&htim2);
    HAL_ADC_Start_IT(&hadc1);
}

uint16_t lem_get_current(void) {
    return current;
}

void lem_new_value(uint16_t val) {
    current = LEM_EXP_SMOOTH_ALPHA*val + (1-LEM_EXP_SMOOTH_ALPHA)*current;
}

void lem_adc_callback(ADC_HandleTypeDef *hadc) {
    lem_new_value(hadc->Instance->DR);
}