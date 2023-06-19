#include "logger_wrapper.h"
#include "usart.h"

char logger_buffer[LOGGER_BUF_LEN];
LOGGER_HandleTypeDef logger_handle;

STMLIBS_StatusTypeDef logger_flush(char *buffer, uint32_t size) {
    if(!__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE))
        return STMLIBS_BUSY;

    HAL_UART_Transmit_IT(&huart1, (uint8_t*)buffer, size);
    return STMLIBS_OK;
}

void logger_init(void) {
    LOGGER_init(&logger_handle, logger_buffer, LOGGER_BUF_LEN, logger_flush);
}

void logger_routine(void) {
    LOGGER_flush(&logger_handle);
}