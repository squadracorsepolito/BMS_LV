/**
 * @file    CONF.h
 * @author  Simone Ruffini [simone.ruffini@squadracorsepolito.com, simone.ruffini.work@gmail.com]
 * @date    Sat May 25 07:37:59 PM CEST 2024
 * @updated Sat May 25 07:37:59 PM CEST 2024
 * @prefix  CONF
 *
 * @brief   Header file for high level code configurations 
 *
 * @license Licensed under "THE BEER-WARE LICENSE", Revision 69 
 *          see LICENSE file in the root directory of this software component
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CONF_H_
#define _CONF_H_

/* ---------- Includes ------------------------------------------------------*/


/* ---------- Exported types ------------------------------------------------*/


/* ---------- Exported constants --------------------------------------------*/
#define VERSION_MAJOR 2
#define VERSION_MINOR 1
#define VERSION_PATCH 1

#define DEBUG_SERIAL
#define DEBUG_AIN
//#define DEBUG_NTC
//#define DEBUG_SDC
//#define DEBUG_BSPD
//#define DEBUG_BRAKE_LIGHT

/* ---------- Exported macros -----------------------------------------------*/
#define GET_VERSION_STR() "v##VERSION_MAJOR.##VERSION_MINOR.##VERSION_PATCH"


/* ---------- Exported functions --------------------------------------------*/


/* ---------- Private types -------------------------------------------------*/



#endif
