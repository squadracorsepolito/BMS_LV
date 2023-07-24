#ifndef CAN_UTILS_H
#define CAN_UTILS_H

#include "can.h"
#include "mcb.h"

void can_init(void);
void can_send_msg(uint32_t id);

#endif // CAN_UTILS_H