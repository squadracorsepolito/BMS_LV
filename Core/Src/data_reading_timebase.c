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

STMLIBS_StatusTypeDef data_reading_l9963e_cb(void) {
  uint16_t const *cells;
  uint8_t cells_n;
  uint8_t is_ntc_measure_required = ntc_is_measure_ext_time();
  L9963E_utils_read_cells(is_ntc_measure_required);

  cells = L9963E_utils_get_cells(&cells_n);

  for(uint8_t i=2; i<5; ++i) {
    if(cells[i] > 4.2 / 0.000089)
      error_set_overvoltage(i);
    else
      error_reset_overvoltage(i);

    if(cells[i] < 2.5 / 0.000089)
      error_set_undervoltage(i);
    else
      error_reset_undervoltage(i);
  }

  if(is_ntc_measure_required && HAL_GetTick() > 5000) {
    for(uint8_t i=0; i<NTC_INT_ADC_N; ++i) {
      if(ntc_get_int_temp(i) > 80){
        error_set_overtemp(i);
      } else if(ntc_get_int_temp(i) < -20){
        error_set_undertemp(i);
      } else{
        error_reset_overtemp(i);
        error_reset_undertemp(i);
      }
    }
    for(uint8_t i=0; i<NTC_EXT_ADC_N; ++i) {
      if(ntc_get_ext_temp(i) > 80){
        error_set_overtemp(NTC_INT_ADC_N+i);
      } else if(ntc_get_ext_temp(i) < -20){
        error_set_undertemp(NTC_INT_ADC_N+i);
      } else {
        error_reset_overtemp(NTC_INT_ADC_N+i);
        error_reset_undertemp(NTC_INT_ADC_N+i);
      }
    }
  }

  return STMLIBS_OK;
}

void data_reading_timebase_init(void) {
  uint8_t interval;

  TIMEBASE_init(&data_reading_timebase_handle, &htim6, 1000);

  TIMEBASE_add_interval(&data_reading_timebase_handle, 10000, &interval);
  TIMEBASE_register_callback(&data_reading_timebase_handle, interval, data_reading_l9963e_cb);
}

void data_reading_timebase_routine(void) {
    TIMEBASE_routine(&data_reading_timebase_handle);
}

void data_reading_timebase_timerElapsed_irq(TIM_HandleTypeDef *htim) {
  TIMEBASE_TimerElapsedCallback(&data_reading_timebase_handle, htim);
}