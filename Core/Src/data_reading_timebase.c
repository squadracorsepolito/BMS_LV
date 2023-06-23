#include "data_reading_timebase.h"
#include "tim.h"

#include <stdio.h>
#include <string.h>
#include "usart.h"

#include "L9963E_utils.h"
#include "lem.h"
#include "ntc.h"
#include "logger_wrapper.h"
#include "errors.h"

TIMEBASE_HandleTypeDef data_reading_timebase_handle;

STMLIBS_StatusTypeDef data_reading_lem_cb(void) {
  logger_log(LOGGER_INFO, "curr: %f", lem_get_current_ampere());
  logger_log(LOGGER_INFO, "temp: %f", ntc_get_int_temp(0));
  return STMLIBS_OK;
}

STMLIBS_StatusTypeDef data_reading_l9963e_cb(void) {
  uint16_t const *cells;
  uint8_t cells_n;
  uint8_t is_ntc_measure_required = ntc_is_measure_ext_time();
  L9963E_utils_read_cells(is_ntc_measure_required);

  cells = L9963E_utils_get_cells(&cells_n);

  for(uint8_t i=0; i<cells_n; ++i) {
    if(cells[i] > 4.2 / 0.000089)
      error_set_overvoltage(i);
    else
      error_reset_overvoltage(i);

    if(cells[i] < 2.8 / 0.000089)
      error_set_undervoltage(i);
    else
      error_reset_undervoltage(i);
  }

  if(is_ntc_measure_required) {
    for(uint8_t i=0; i<NTC_INT_ADC_N; ++i) {
      if(ntc_get_int_temp(i) > 80)
        error_set_overtemp(i);
      else
        error_reset_overtemp(i);
    }
    for(uint8_t i=0; i<NTC_EXT_ADC_N; ++i) {
      if(ntc_get_ext_temp(i) > 80)
        error_set_overtemp(NTC_EXT_ADC_N+i);
      else
        error_reset_overtemp(NTC_EXT_ADC_N+i);
    }
  }

  return STMLIBS_OK;
}

void data_reading_timebase_init(void) {
  uint8_t interval;

  TIMEBASE_init(&data_reading_timebase_handle, &htim6, 10000);

  TIMEBASE_add_interval(&data_reading_timebase_handle, 10000, &interval);
  TIMEBASE_register_callback(&data_reading_timebase_handle, interval, data_reading_l9963e_cb);

  TIMEBASE_add_interval(&data_reading_timebase_handle, 1000000, &interval);
  TIMEBASE_register_callback(&data_reading_timebase_handle, interval, data_reading_lem_cb);
}

void data_reading_timebase_routine(void) {
    TIMEBASE_routine(&data_reading_timebase_handle);
}