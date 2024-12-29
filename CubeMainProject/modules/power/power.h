/**
 * @file power.h
 * @author Maksym T. (torgunakovmaks2@gmail.com)
 * @brief This module manages power.
 * 
 * @version 0.1 @date 2024-07-07
 * 
 * @version 0.2 @date 
 * - Low power mode implemented.
 * 
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _POWER_POWER_H_
#define _POWER_POWER_H_

#include "sys.h"

#define PWR_EN_PORT         GPIOA
#define PWR_EN_PIN          GPIO_PIN_4

#define PWR_CHARGE_PORT     GPIOA
#define PWR_CHARGE_PIN      GPIO_PIN_3
/*=================================================================*/

#define PWR_ENABLE()        HAL_GPIO_WritePin(PWR_EN_PORT, PWR_EN_PIN, GPIO_PIN_SET)
#define PWR_DISABLE()       HAL_GPIO_WritePin(PWR_EN_PORT, PWR_EN_PIN, GPIO_PIN_RESET)       
/*=================================================================*/

typedef enum {
    PWR_State_OK,
    PWR_State_NOK,
    PWR_State_Busy,
} PWR_State_t;

typedef enum {
    PWR_PowerMode_Normal,
    PWR_PowerMode_LowPower,
    PWR_PowerMode_ToLowPower,
    PWR_PowerMode_ToNormal,
} PWR_PowerMode_t;
/*=================================================================*/

void PWR_GPIO_Init(void);

/**
 * @brief 
 * 
 * @return true --> battery is charging at the moment.
 * @return false --> battery is not charging.
 */
bool PWR_IsChargeActive(void);

void PWR_PowerMode_IncreaseCounter(uint32_t ms);
void PWR_PowerMode_ResetCounter(void);
void PWR_PowerMode_SetCounterLimit(uint32_t ms);
void PWR_PowerMode_EnterLowPowerMode_Notify(void);
void PWR_PowerMode_EnterNormalMode_Notify(void);

void PWR_Cyclic(void);
/*=================================================================*/

#endif //_POWER_POWER_H_
