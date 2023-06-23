#include "hooks.h"

blt_bool CpuUserProgramStartHook() {
    HAL_CAN_DeInit(&hcan1);
    return 1;
}