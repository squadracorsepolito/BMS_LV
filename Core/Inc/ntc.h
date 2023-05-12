#ifndef NTC_H
#define NTC_H

#include "adc.h"

void ntc_init(void);
uint8_t ntc_is_measure_ext_time(void);
void ntc_set_data(uint16_t const *data, uint8_t len, uint8_t offset);
void ntc_adc_callback(ADC_HandleTypeDef *hadc);

#endif // NTC_H
