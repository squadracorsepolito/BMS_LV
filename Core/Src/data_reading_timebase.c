#include "data_reading_timebase.h"
#include "tim.h"

#include <stdio.h>
#include <string.h>
#include "usart.h"

#include "L9963E_utils.h"
#include "lem.h"
#include "ntc.h"
#include "logger_wrapper.h"

TIMEBASE_HandleTypeDef data_reading_timebase_handle;

STMLIBS_StatusTypeDef data_reading_lem_cb(void) {
  logger_log(LOGGER_INFO, "%f", ((lem_get_current() * 3.3 / 4095) - 2.5) / 0.0125);
  return STMLIBS_OK;
}

STMLIBS_StatusTypeDef data_reading_l9963e_cb(void) {
  uint8_t is_ntc_measure_required = ntc_is_measure_ext_time();
  L9963E_utils_read_cells(is_ntc_measure_required);
  return STMLIBS_OK;
}

STMLIBS_StatusTypeDef print_cells(void) {
  uint16_t const *cells;
  cells = L9963E_utils_get_cells(NULL);
  
  for(int i=0; i<7; ++i) {
    logger_log(LOGGER_INFO, "c%d: %f", i, cells[i] * 0.000089);
  }

  return STMLIBS_OK;
}

void data_reading_timebase_init(void) {
  uint8_t interval;

  TIMEBASE_init(&data_reading_timebase_handle, &htim6, 1000);

  TIMEBASE_add_interval(&data_reading_timebase_handle, 100000, &interval);
  TIMEBASE_register_callback(&data_reading_timebase_handle, interval, data_reading_lem_cb);

  // TIMEBASE_add_interval(&data_reading_timebase_handle, 200000, &interval);
  // TIMEBASE_register_callback(&data_reading_timebase_handle, interval, data_reading_l9963e_cb);

  // TIMEBASE_add_interval(&data_reading_timebase_handle, 500000, &interval);
  // TIMEBASE_register_callback(&data_reading_timebase_handle, interval, print_cells);
}

void data_reading_timebase_routine(void) {
    TIMEBASE_routine(&data_reading_timebase_handle);
}