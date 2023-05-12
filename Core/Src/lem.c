#include "lem.h"

#include "tim.h"

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
    current = 0.1*val + 0.9*current;
}

void lem_adc_callback(ADC_HandleTypeDef *hadc) {
    lem_new_value(hadc->Instance->DR);
}