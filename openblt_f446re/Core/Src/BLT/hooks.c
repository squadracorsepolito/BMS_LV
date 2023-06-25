#include "hooks.h"

blt_bool CpuUserProgramStartHook() {
    // HAL_CAN_DeInit(&canHandle);
    HAL_RCC_DeInit();
    return 1;
}