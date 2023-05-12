#include "L9963E_utils.h"

#include "L9963E.h"
#include "stm32_if.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

L9963E_HandleTypeDef h9l;
#define ENABLED_CELLS (L9963E_CELL1 | L9963E_CELL2 | L9963E_CELL3 | L9963E_CELL4 | L9963E_CELL12 | L9963E_CELL13 | L9963E_CELL14)

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
  
  L9963E_setCommTimeout_Broadcast(&h9l, _256MS);
  L9963E_set_enabled_cells(&h9l, 0x1, ENABLED_CELLS);
}

void L9963E_utils_read_cells(uint8_t read_gpio) {
  L9963E_StatusTypeDef e;
  char out[64];
  L9963E_start_conversion(&h9l, 0x1, read_gpio ? L9963E_GPIO_CONV : 0);

  uint8_t c_done = 0;
  do {
    e = L9963E_poll_conversion(&h9l, 0x1, &c_done);
  } while(!c_done || e != L9963E_OK);
  
  uint16_t vcell = 0;
  uint8_t d_rdy = 0;

  L9963E_read_cell_voltage(&h9l, 0x1, L9963E_CELL1, &vcell, &d_rdy);
  snprintf(out, 64, "1 --> ready: %d, data: %fV\r\n", d_rdy, vcell*0.000089);
  HAL_UART_Transmit(&huart1, (uint8_t*)out, strlen(out), 100);

  L9963E_read_cell_voltage(&h9l, 0x1, L9963E_CELL2, &vcell, &d_rdy);
  snprintf(out, 64, "2 --> ready: %d, data: %fV\r\n", d_rdy, vcell*0.000089);
  HAL_UART_Transmit(&huart1, (uint8_t*)out, strlen(out), 100);

  L9963E_read_cell_voltage(&h9l, 0x1, L9963E_CELL3, &vcell, &d_rdy);
  snprintf(out, 64, "3 --> ready: %d, data: %fV\r\n", d_rdy, vcell*0.000089);
  HAL_UART_Transmit(&huart1, (uint8_t*)out, strlen(out), 100);

  L9963E_read_cell_voltage(&h9l, 0x1, L9963E_CELL4, &vcell, &d_rdy);
  snprintf(out, 64, "4 --> ready: %d, data: %fV\r\n", d_rdy, vcell*0.000089);
  HAL_UART_Transmit(&huart1, (uint8_t*)out, strlen(out), 100);

  L9963E_read_cell_voltage(&h9l, 0x1, L9963E_CELL12, &vcell, &d_rdy);
  snprintf(out, 64, "12 --> ready: %d, data: %fV\r\n", d_rdy, vcell*0.000089);
  HAL_UART_Transmit(&huart1, (uint8_t*)out, strlen(out), 100);

  L9963E_read_cell_voltage(&h9l, 0x1, L9963E_CELL13, &vcell, &d_rdy);
  snprintf(out, 64, "13 --> ready: %d, data: %fV\r\n", d_rdy, vcell*0.000089);
  HAL_UART_Transmit(&huart1, (uint8_t*)out, strlen(out), 100);

  L9963E_read_cell_voltage(&h9l, 0x1, L9963E_CELL14, &vcell, &d_rdy);
  snprintf(out, 64, "14 --> ready: %d, data: %fV\r\n", d_rdy, vcell*0.000089);
  HAL_UART_Transmit(&huart1, (uint8_t*)out, strlen(out), 100);
}