#include "L9963E_utils.h"

#include "L9963E.h"
#include "stm32_if.h"
#include "ntc.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

#include "logger_wrapper.h"

L9963E_HandleTypeDef h9l;
volatile uint16_t vcells[CELLS_N];
volatile uint16_t vgpio[GPIOS_N];
volatile uint16_t vtot;

const L9963E_IfTypeDef interface = {
    .L9963E_IF_DelayMs = DelayMs,
    .L9963E_IF_GetTickMs = GetTickMs,
    .L9963E_IF_GPIO_ReadPin = GPIO_ReadPin,
    .L9963E_IF_GPIO_WritePin = GPIO_WritePin,
    .L9963E_IF_SPI_Receive = SPI_Receive,
    .L9963E_IF_SPI_Transmit = SPI_Transmit
};

void L9963E_utils_init(void) {
  L9963E_init(&h9l, interface, 1);
  L9963E_addressing_procedure(&h9l, 0b11, 0, 0, 1);

  L9963E_RegisterUnionTypeDef gpio9_3_conf_reg = {.generic = L9963E_GPIO9_3_CONF_DEFAULT};
  gpio9_3_conf_reg.GPIO9_3_CONF.GPIO7_CONFIG = 0;
  gpio9_3_conf_reg.GPIO9_3_CONF.GPIO8_CONFIG = 0;
  L9963E_DRV_reg_write(&(h9l.drv_handle), L9963E_DEVICE_BROADCAST, L9963E_GPIO9_3_CONF_ADDR, &gpio9_3_conf_reg, 10);

  L9963E_RegisterUnionTypeDef vcell_thresh_uv_ov_reg = {.generic = L9963E_VCELL_THRESH_UV_OV_DEFAULT};
  vcell_thresh_uv_ov_reg.VCELL_THRESH_UV_OV.threshVcellOV = 0xff;
  L9963E_DRV_reg_write(&(h9l.drv_handle), L9963E_DEVICE_BROADCAST, L9963E_VCELL_THRESH_UV_OV_ADDR, &vcell_thresh_uv_ov_reg, 10);

  L9963E_RegisterUnionTypeDef vbat_sum_th_reg = {.generic = L9963E_VBATT_SUM_TH_DEFAULT};
  vbat_sum_th_reg.VBATT_SUM_TH.VBATT_SUM_OV_TH = 0xff;
  L9963E_DRV_reg_write(&(h9l.drv_handle), L9963E_DEVICE_BROADCAST, L9963E_VBATT_SUM_TH_ADDR, &vbat_sum_th_reg, 10);

  L9963E_enable_vref(&h9l, L9963E_DEVICE_BROADCAST, 0);
  
  L9963E_setCommTimeout(&h9l, _256MS, L9963E_DEVICE_BROADCAST, 0);
  L9963E_set_enabled_cells(&h9l, 0x1, ENABLED_CELLS);
}

void L9963E_utils_read_cells(uint8_t read_gpio) {
  L9963E_StatusTypeDef e;
  uint8_t c_done;
  do {
    L9963E_poll_conversion(&h9l, 0x1, &c_done);
  } while (!c_done);
  L9963E_start_conversion(&h9l, 0x1, read_gpio ? L9963E_GPIO_CONV : 0);
  
  uint16_t voltage = 0;
  uint8_t d_rdy = 0;

  do {
    e = L9963E_read_cell_voltage(&h9l, 0x1, L9963E_CELL1, &voltage, &d_rdy);
  } while(e != L9963E_OK || !d_rdy);
  vcells[0] = voltage;

  do {
    e = L9963E_read_cell_voltage(&h9l, 0x1, L9963E_CELL2, &voltage, &d_rdy);
  } while(e != L9963E_OK || !d_rdy);
  vcells[1] = voltage;

  do {
    e = L9963E_read_cell_voltage(&h9l, 0x1, L9963E_CELL3, &voltage, &d_rdy);
  } while(e != L9963E_OK || !d_rdy);
  vcells[2] = voltage;

  do {
    e = L9963E_read_cell_voltage(&h9l, 0x1, L9963E_CELL4, &voltage, &d_rdy);
  } while(e != L9963E_OK || !d_rdy);
  vcells[3] = voltage;

  do {
    e = L9963E_read_cell_voltage(&h9l, 0x1, L9963E_CELL12, &voltage, &d_rdy);
  } while(e != L9963E_OK || !d_rdy);
  vcells[4] = voltage;

  do {
    e = L9963E_read_cell_voltage(&h9l, 0x1, L9963E_CELL13, &voltage, &d_rdy);
  } while(e != L9963E_OK || !d_rdy);
  vcells[5] = voltage;

  do {
    e = L9963E_read_cell_voltage(&h9l, 0x1, L9963E_CELL14, &voltage, &d_rdy);
  } while(e != L9963E_OK || !d_rdy);
  vcells[6] = voltage;

  do {
    e = L9963E_read_batt_voltage(&h9l, 0x1, &voltage);
  } while(e != L9963E_OK);
  vtot = voltage;

  if(!read_gpio)
    return;

  do {
    e = L9963E_read_gpio_voltage(&h9l, 0x1, L9963E_GPIO3, &voltage, &d_rdy);
  } while(e != L9963E_OK || !d_rdy);
  vgpio[0] = voltage;

  do {
    e = L9963E_read_gpio_voltage(&h9l, 0x1, L9963E_GPIO4, &voltage, &d_rdy);
  } while(e != L9963E_OK || !d_rdy);
  vgpio[1] = voltage;

  do {
    e = L9963E_read_gpio_voltage(&h9l, 0x1, L9963E_GPIO5, &voltage, &d_rdy);
  } while(e != L9963E_OK || !d_rdy);
  vgpio[2] = voltage;

  do {
    e = L9963E_read_gpio_voltage(&h9l, 0x1, L9963E_GPIO6, &voltage, &d_rdy);
  } while(e != L9963E_OK || !d_rdy);
  vgpio[3] = voltage;

  do {
    e = L9963E_read_gpio_voltage(&h9l, 0x1, L9963E_GPIO7, &voltage, &d_rdy);
  } while(e != L9963E_OK || !d_rdy);
  vgpio[4] = voltage;

  do {
    e = L9963E_read_gpio_voltage(&h9l, 0x1, L9963E_GPIO8, &voltage, &d_rdy);
  } while(e != L9963E_OK || !d_rdy);
  vgpio[5] = voltage;

  do {
    e = L9963E_read_gpio_voltage(&h9l, 0x1, L9963E_GPIO9, &voltage, &d_rdy);
  } while(e != L9963E_OK || !d_rdy);
  vgpio[6] = voltage;

  ntc_set_ext_data(vgpio, GPIOS_N, 0);
}

uint16_t const* L9963E_utils_get_gpios(uint8_t *len) {
  if(len)
    *len = GPIOS_N;
  return vgpio;
}

uint16_t const* L9963E_utils_get_cells(uint8_t *len) {
  if(len)
    *len = CELLS_N;
  return vcells;
}

float L9963E_utils_get_cell_mv(uint8_t index) {
  return vcells[index] * 89e-3f;
}

float L9963E_utils_get_batt_mv(void) {
  return vtot * 1.33f;
}