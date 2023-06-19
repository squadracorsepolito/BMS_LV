#ifndef LEM_H
#define LEM_H

#include "adc.h"
#include <inttypes.h>

void lem_init(void);
float lem_get_current(void);
void lem_new_value(uint16_t val);
void lem_adc_callback(ADC_HandleTypeDef *hadc);

#endif // LEM_H
