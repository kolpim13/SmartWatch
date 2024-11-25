/**
 * @file RTC.h
 * @author Maksym T. (torgunakovmaks2@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-09
 * 
 * @copyright Copyright (c) 2024
 * 
 * @todo Try to store data in Binary format for more easy readout.
 */

#ifndef RTC_RTC_H_
#define RTC_RTC_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#define RTC_STATE_BKP_REGISTER      RTC_BKP_DR1
#define RTC_STATE_ENABLE            0xABCD
/*=================================================================*/

#define RTC_TIME_FORMAT_BIN         RTC_FORMAT_BIN
#define RTC_TIME_FORMAT_BCD         RTC_FORMAT_BCD
#define RTC_TIME_FORMAT             RTC_TIME_FORMAT_BIN
/*=================================================================*/

/**
 * @brief 
 * @todo 
 *  1. Read data from eeprom should be checked.
 *  2. Read data from NvM OR setup default values, NOT combined version.
 */
void RTC_Init(void);

/**
 * @brief 
 * 
 * @param sTime 
 * @param str_out 
 */
void RTC_ConvertTimeToString(RTC_TimeTypeDef* sTime, char* str_out, size_t );

/**
 * @brief 
 * 
 * @param sTime 
 * @param str_out 
 */
void RTC_ConvertDateToString(RTC_TimeTypeDef* sTime, char* str_out);

#endif //RTC_RTC_H_
