#ifndef DATA_READING_TIMEBASE_H
#define DATA_READING_TIMEBASE_H

#include "timebase.h"

extern TIMEBASE_HandleTypeDef data_reading_timebase_handle;

void data_reading_timebase_init(void);
void data_reading_timebase_routine(void);

#endif // DATA_READING_TIMEBASE_H
