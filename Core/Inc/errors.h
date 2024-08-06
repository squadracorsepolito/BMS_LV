#ifndef ERRORS_H
#define ERRORS_H

#include "error_utils.h"
#include "tim.h"

typedef enum {
    ERRORS_UNDERVOLTAGE,
    ERRORS_OVERVOLTAGE,
    ERRORS_OVERTEMPERATURE,
    ERRORS_UNDERTEMPERATURE,
    ERRORS_NUM
} ERRORS_TypeDef;

void error_init(void);

void error_timerElapsed_irq(TIM_HandleTypeDef *htim);

void error_set_undervoltage(uint8_t index);
void error_set_overvoltage(uint8_t index);
void error_set_overtemp(uint8_t index);
void error_set_undertemp(uint8_t index);

void error_reset_undervoltage(uint8_t index);
void error_reset_overvoltage(uint8_t index);
void error_reset_overtemp(uint8_t index);
void error_reset_undertemp(uint8_t index);

uint8_t error_get_overvoltage(uint8_t index);
uint8_t error_get_undervoltage(uint8_t index);
uint8_t error_get_overtemp(uint8_t index);
uint8_t error_get_undertemp(uint8_t index);

#endif // ERRORS_H
