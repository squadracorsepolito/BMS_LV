/**
 * @file    app_db.h
 * @author  Simone Ruffini [siomne.ruffini@squadracorsepolito.com | simone.ruffini.work@gmail.com]
 * @date    Wed Aug 28 11:45:41 AM CEST 2024
 * @updated Wed Aug 28 11:45:41 AM CEST 2024 
 * @prefix  ADB
 *
 * @brief   Implementation of Application DataBase header file
 *
 * @license Licensed under "THE BEER-WARE LICENSE", Revision 69 
 *          see LICENSE file in the root directory of this software component
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _ADB_H_
#define _ADB_H_

/* ---------- Includes -------------------------------------------------------*/
#include <stdint.h>

/* ---------- Exported types -------------------------------------------------*/

enum LV_RLY_override {
    LV_RLY_override_OFF,
    LV_RLY_override_OPEN,
    LV_RLY_override_CLOSED,
};

struct ADB_data {
    uint8_t DIAG_mode_status;
    enum LV_RLY_override LV_RLY_override_state;
};


/* ---------- Exported constants ---------------------------------------------*/


/* ---------- Exported variables ---------------------------------------------*/


/* ---------- Exported macros ------------------------------------------------*/

extern volatile struct ADB_data ADB;

/* ---------- Exported functions ---------------------------------------------*/


/* ---------- Private types --------------------------------------------------*/


/* ---------- Private variables ----------------------------------------------*/


/* ---------- Private constants ----------------------------------------------*/


/* ---------- Private Macros -------------------------------------------------*/


#endif
