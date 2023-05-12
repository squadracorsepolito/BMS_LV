#include "data_reading_timebase.h"
#include "tim.h"

#include <stdio.h>
#include <string.h>
#include "usart.h"

#include "L9963E_utils.h"
#include "lem.h"
#include "ntc.h"

TIMEBASE_HandleTypeDef data_reading_timebase_handle;

STMLIBS_StatusTypeDef data_reading_lem_cb(void) {
  char out[32];
  sprintf(out, "%d\r\n", lem_get_current());
  HAL_UART_Transmit(&huart1, (uint8_t*)out, strlen(out), 100);
  return STMLIBS_OK;
}

STMLIBS_StatusTypeDef data_reading_l9963e_cb(void) {
  uint8_t is_ntc_measure_required = ntc_is_measure_ext_time();
  L9963E_utils_read_cells(is_ntc_measure_required);

  if(is_ntc_measure_required)
    ntc_acquire_ext_data();
  return STMLIBS_OK;
}

void data_reading_timebase_init(void) {
  uint8_t interval;

  TIMEBASE_init(&data_reading_timebase_handle, &htim6, 1000);
  
  TIMEBASE_add_interval(&data_reading_timebase_handle, 100000, &interval);
  TIMEBASE_register_callback(&data_reading_timebase_handle, interval, data_reading_lem_cb);

  TIMEBASE_add_interval(&data_reading_timebase_handle, 200000, &interval);
  TIMEBASE_register_callback(&data_reading_timebase_handle, interval, data_reading_l9963e_cb);
}

void data_reading_timebase_routine(void) {
    TIMEBASE_routine(&data_reading_timebase_handle);
}