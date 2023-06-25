#ifndef DATA_READING_TIMEBASE_H
#define DATA_READING_TIMEBASE_H

#include "timebase.h"
#include "tim.h"

void data_reading_timebase_init(void);
void data_reading_timebase_routine(void);
void data_reading_timebase_timerElapsed_irq(TIM_HandleTypeDef *htim);

#endif // DATA_READING_TIMEBASE_H
