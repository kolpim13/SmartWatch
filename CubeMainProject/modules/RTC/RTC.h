/**
 * @file RTC.h
 * @author Maksym T. (torgunakovmaks2@gmail.com)
 * @brief 
 * @version 0.1 @date 2024-07-09
 *  - Module created
 * 
 * @version 0.2 @date 2024-11-25
 *  - Now RTC use internal variables to store 
 *  - Define RTC_TIME_FORMAT introduced to unify time format accross the project
 * 
 * 
 * @copyright Copyright (c) 2024
 * 
 * @todo Try to store data in Binary format for more easy readout.
 */

#ifndef RTC_RTC_H_
#define RTC_RTC_H_

#include "sys.h"

#define RTC_STATE_BKP_REGISTER      RTC_BKP_DR1
#define RTC_STATE_ENABLE            0xABCD
/*=================================================================*/

#define RTC_TIME_FORMAT_BIN         RTC_FORMAT_BIN
#define RTC_TIME_FORMAT_BCD         RTC_FORMAT_BCD
#define RTC_TIME_FORMAT             RTC_TIME_FORMAT_BCD
/*=================================================================*/

/**
 * @brief 
 * @todo 
 *  1. Read data from eeprom should be checked.
 *  2. Read data from NvM OR setup default values, NOT combined version.
 */
void RTC_Init(void);

/**
 * @brief Updates states of internal variables sTime and sDate.
 * Should be called cyclically, at least once in second [but not too often].
 * 
 * @todo Measure the time needed to update sime and sDate, along with corresponding strings [Use debug pins].
 *  Try to think how to optimize these operations.
 */
void RTC_Cyclic_1s(void);
/*=================================================================*/


#endif //RTC_RTC_H_
