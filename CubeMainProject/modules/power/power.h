/**
 * @file power.h
 * @author Maksym T. (torgunakovmaks2@gmail.com)
 * @brief This is a power manager.
 * It responsible for Low Power and Sleep modes, battery status and control.
 * In Low power mode device remain active but slightly reduce its power consumption:
 *  - Main core and all peripheral remain active.
 *  - All sensors works without changes.
 *  - Display is disabled + LVGL task is not halted.
 *  - Touch controller is in standby mode --> Device can be waken via touching it.
 *  - Main frequency slightly decreased (?) [Since the most computationally consuming part (LVGL) is not active].
 * 
 * @version 0.1 @date 2024-07-07
 *  - Place holder for module added.
 * 
 * @version 0.2 @date 2024-11-28
 *  - Low power mode introduced
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

/**
 * @brief 
 * 
 */
void PWR_LowPowerMode_Enter(void);

/**
 * @brief 
 * 
 */
void PWR_LowPowerMode_Exit(void);

/**
 * @brief TBD
 * 
 */
void PWR_SleepMode_Enter(void);

/**
 * @brief TBD
 * 
 */
void PWR_SleepMode_Exit(void);
/*=================================================================*/

#endif //_POWER_POWER_H_
