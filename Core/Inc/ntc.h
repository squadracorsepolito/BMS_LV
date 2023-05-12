#ifndef NTC_H
#define NTC_H

#include "adc.h"

void ntc_init(void);
uint8_t ntc_is_measure_ext_time(void);
void ntc_acquire_ext_data(void);
void ntc_adc_callback(ADC_HandleTypeDef *hadc);

#endif // NTC_H
