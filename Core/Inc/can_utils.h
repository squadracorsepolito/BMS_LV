#ifndef CAN_UTILS_H
#define CAN_UTILS_H

#include "can.h"
#include "conf.h"
#include "../../Lib/SCan/SC26/artifacts/MCB/c_source/mcb.h"

void MCB_send_msg(uint32_t id);

#endif // CAN_UTILS_H
