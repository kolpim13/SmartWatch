/**
 * @file power.h
 * @author Maksym T. (torgunakovmaks2@gmail.com)
 * @brief This module manages power 
 * @version 0.1
 * @date 2024-07-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _POWER_POWER_H_
#define _POWER_POWER_H_

#include "sys.h"

#define POWER_EN_PORT         GPIOA
#define POWER_EN_PIN          GPIO_PIN_4
/*=================================================================*/

#define POWER_ENABLE()        HAL_GPIO_WritePin(POWER_EN_PORT, POWER_EN_PIN, GPIO_PIN_SET)
#define POWER_DISABLE()       HAL_GPIO_WritePin(POWER_EN_PORT, POWER_EN_PIN, GPIO_PIN_RESET)       
/*=================================================================*/

void POWER_GPIO_Init(void);

#endif //_POWER_POWER_H_
