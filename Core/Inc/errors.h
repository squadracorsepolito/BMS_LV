#ifndef ERRORS_H
#define ERRORS_H

#include "error_utils.h"
extern ERROR_UTILS_HandleTypeDef error_h;

typedef enum {
    ERRORS_UNDERVOLTAGE,
    ERRORS_OVERVOLTAGE,
    ERRORS_OVERTEMPERATURE,
    ERRORS_NUM
} ERRORS_TypeDef;

void error_init(void);
void error_set_undervoltage(uint8_t index);
void error_set_overvoltage(uint8_t index);
void error_set_overtemp(uint8_t index);
void error_reset_undervoltage(uint8_t index);
void error_reset_overvoltage(uint8_t index);
void error_reset_overtemp(uint8_t index);
#endif // ERRORS_H
