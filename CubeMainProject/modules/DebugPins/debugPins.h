/**
 * @file debugPins.h
 * @author Maksym T. (torgunakovmaks2@gmail.com)
 * @brief Module used for debug purposes.
 *      Drives HW pins, so they can be observed by external HW / SW. [e.g. Logic, osciloscop, etc.]
 * @version 0.1 @date 2024-11-23
 *  - Was created on debug purpose for HW V1.1
 * 
 * @todo Should be tested on the real HW.
 * 
 */

#ifndef _DEBUGPINS_H_
#define _DEBUGPINS_H_

#include "sys.h"
/*=================================================================*/

/**
 * @brief Layout and configuration of the debug pins can be determened by HW version 
 * 
 */

#if HW_VERSION == HW_V1_0 || HW_VERSION == HW_V1_1
#define DEBUG_PIN_0         GPIO_PIN_12
#define DEBUG_PIN_0_PORT    GPIOA

#define DEBUG_PIN_1         GPIO_PIN_11
#define DEBUG_PIN_1_PORT    GPIOA

#define DEBUG_PIN_2         GPIO_PIN_15
#define DEBUG_PIN_2_PORT    GPIOB

#define DEBUG_PIN_3         GPIO_PIN_14
#define DEBUG_PIN_3_PORT    GPIOB

#define DEBUG_PIN_4         GPIO_PIN_13
#define DEBUG_PIN_4_PORT    GPIOB
#endif
/*=================================================================*/

#define DebugPin_Set(PORT, PIN)     HAL_GPIO_WritePin(PORT, PIN, GPIO_PIN_SET)
#define DebugPin_Reset(PORT, PIN)   HAL_GPIO_WritePin(PORT, PIN, GPIO_PIN_RESET)
/*=================================================================*/

#define DEBUG_PIN_0_SET()       DebugPin_Set(DEBUG_PIN_0_PORT, DEBUG_PIN_0)
#define DEBUG_PIN_0_RESET()     DebugPin_Reset(DEBUG_PIN_0_PORT, DEBUG_PIN_0)

#define DEBUG_PIN_1_SET()       DebugPin_Set(DEBUG_PIN_1_PORT, DEBUG_PIN_1)
#define DEBUG_PIN_1_RESET()     DebugPin_Reset(DEBUG_PIN_1_PORT, DEBUG_PIN_1)

#define DEBUG_PIN_2_SET()       DebugPin_Set(DEBUG_PIN_2_PORT, DEBUG_PIN_2)
#define DEBUG_PIN_2_RESET()     DebugPin_Reset(DEBUG_PIN_2_PORT, DEBUG_PIN_2)

#define DEBUG_PIN_3_SET()       DebugPin_Set(DEBUG_PIN_3_PORT, DEBUG_PIN_3)
#define DEBUG_PIN_3_RESET()     DebugPin_Reset(DEBUG_PIN_3_PORT, DEBUG_PIN_3)

#define DEBUG_PIN_4_SET()       DebugPin_Set(DEBUG_PIN_4_PORT, DEBUG_PIN_4)
#define DEBUG_PIN_4_RESET()     DebugPin_Reset(DEBUG_PIN_4_PORT, DEBUG_PIN_4)
/*=================================================================*/

void DebugPins_GPIO_Init(void);
/*=================================================================*/

#endif //_DEBUGPINS_H_
