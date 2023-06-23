#ifndef NTC_H
#define NTC_H

#include "adc.h"

#define NTC_EXT_ADC_N 7
#define NTC_INT_ADC_N 5

void ntc_init(void);
uint8_t ntc_is_measure_ext_time(void);
void ntc_set_ext_data(uint16_t const *data, uint8_t len, uint8_t offset);
void ntc_adc_callback(ADC_HandleTypeDef *hadc);
float ntc_get_ext_resistance(uint8_t index);
float ntc_get_int_resistance(uint8_t index);
float ntc_get_ext_temp(uint8_t index);
float ntc_get_int_temp(uint8_t index);

#endif // NTC_H
