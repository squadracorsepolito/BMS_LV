#ifndef CAN_SEND_TIMEBASE_H
#define CAN_SEND_TIMEBASE_H

#include "timebase.h"
#include "tim.h"

void can_send_timebase_init(void);
void can_send_timebase_routine(void);
void can_send_timebase_timerElapsed_irq(TIM_HandleTypeDef *htim);

#endif // CAN_SEND_TIMEBASE_H
