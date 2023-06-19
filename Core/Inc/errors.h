#ifndef ERRORS_H
#define ERRORS_H

#include "error_utils.h"
extern ERROR_UTILS_HandleTypeDef error_h;

typedef enum {
    ERRORS_UNDERVOLTAGE,
    ERRORS_NUM
} ERRORS_TypeDef;

void error_init(void);
void error_set_undervoltage(uint8_t index);
#endif // ERRORS_H
