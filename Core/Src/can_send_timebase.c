#include "can_send_timebase.h"
#include "can_utils.h"

TIMEBASE_HandleTypeDef can_send_timebase_handle;

STMLIBS_StatusTypeDef battery_pack_general_cb(void) {
  can_send_msg(MCB_BMSLV_BATTERY_PACK_GENERAL_FRAME_ID);
  return STMLIBS_OK;
}

STMLIBS_StatusTypeDef cell_voltage_cb(void) {
  can_send_msg(MCB_BMSLV_CELL_VOLTAGE1_FRAME_ID);
  can_send_msg(MCB_BMSLV_CELL_VOLTAGE2_FRAME_ID);
  return STMLIBS_OK;
}

STMLIBS_StatusTypeDef status_cb(void) {
  can_send_msg(MCB_BMSLV_STATUS_FRAME_ID);
  return STMLIBS_OK;
}

STMLIBS_StatusTypeDef temp_cb(void) {
  can_send_msg(MCB_BMSLV_TEMP1_FRAME_ID);
  can_send_msg(MCB_BMSLV_TEMP2_FRAME_ID);
  return STMLIBS_OK;
}

void can_send_timebase_init(void) {
  uint8_t interval;

  TIMEBASE_init(&can_send_timebase_handle, &htim10, 1000);

  TIMEBASE_add_interval(&can_send_timebase_handle, 1000, &interval);
  TIMEBASE_register_callback(&can_send_timebase_handle, interval, battery_pack_general_cb);
  TIMEBASE_register_callback(&can_send_timebase_handle, interval, cell_voltage_cb);
  TIMEBASE_register_callback(&can_send_timebase_handle, interval, status_cb);

  TIMEBASE_add_interval(&can_send_timebase_handle, 20000, &interval);
  TIMEBASE_register_callback(&can_send_timebase_handle, interval, temp_cb);
}

void can_send_timebase_routine(void) {
  TIMEBASE_routine(&can_send_timebase_handle);
}

void can_send_timebase_timerElapsed_irq(TIM_HandleTypeDef *htim) {
  TIMEBASE_TimerElapsedCallback(&can_send_timebase_handle, htim);
}