/**
 * @file power.h
 * @author Maksym T. (torgunakovmaks2@gmail.com)
 * @brief This module manages power.
 * @version 0.1
 * @date 2024-07-07
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

void PWR_GPIO_Init(void);

/**
 * @brief 
 * 
 * @return true --> battery is charging at the moment.
 * @return false --> battery is not charging.
 */
bool PWR_IsChargeActive(void);
/*=================================================================*/

#endif //_POWER_POWER_H_
