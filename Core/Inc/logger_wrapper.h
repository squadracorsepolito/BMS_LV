#ifndef LOGGER_WRAPPER_H
#define LOGGER_WRAPPER_H

#include "logger.h"

#define LOGGER_BUF_LEN 4096
extern LOGGER_HandleTypeDef logger_handle;

void logger_init(void);
void logger_routine(void);
#define logger_log(mode, template, ...) LOGGER_log(&logger_handle, mode, template __VA_OPT__(,) __VA_ARGS__)

#endif // LOGGER_WRAPPER_H