/**
 * @file RTC.h
 * @author Maksym T. (torgunakovmaks2@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef RTC_RTC_H_
#define RTC_RTC_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#define RTC_STATE_BKP_REGISTER      RTC_BKP_DR1
#define RTC_STATE_ENABLE            0xABCD

/**
 * @brief 
 * @todo 
 *  1. Read data from eeprom should be checked.
 *  2. Read data from NvM OR setup default values, NOT combined version.
 */
void RTC_Init(void);

#endif //RTC_RTC_H_
