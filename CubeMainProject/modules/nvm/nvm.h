/**
 * @file nvm.h
 * @author Maksym Torhunakov (torgunakovmaks2@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-05
 * 
 * @copyright Copyright (c) 2024
 * 
 * @todo Add validity mechanism
 * @todo Add mechanizm to define if this eeprom is setup for the first time.
 *      If YES ==> write default values into eeprom.
 */

#ifndef NvM_NvM_H_
#define NvM_NvM_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#include "BL24C02F.h"

#define NvM_LEN         BL24C02F_MAX_LEN
#define NvM_DATA_LEN    (NvM_LEN - sizeof(NvM_validity_t))

typedef enum 
{
    NvM_Status_OK = 0,
    NvM_Status_NotReady,
    NvM_Status_Error,
} NvM_Status_e;

typedef enum
{
    NvM_Block_Time = (uint8_t)0,
    NvM_Block_Date,
    NvM_Block_Force_U8 = (uint8_t)0xFFu
} NvM_Block_t;

typedef struct NvM_validity_t
{
    uint16_t CNT;
    uint16_t PP;
} NvM_validity_t;

typedef struct NvM_Time_t
{
    uint8_t Hours;
    uint8_t Minutes;
    uint8_t Seconds;
    uint8_t TimeFormat;
} NvM_Time_t;

typedef struct NvM_Date_t
{
    uint8_t WeekDay;
    uint8_t Mounth;
    uint8_t Date;
    uint8_t Year;
} NvM_Date_t;

/**
 * @brief Structure should be 8 bytes aligned (As chip is 8-byte alligned).
 * 
 */
typedef struct NvM_config_t
{
    union
    {
        uint8_t raw_bytes[NvM_DATA_LEN];
        struct
        {
            NvM_Time_t time;    // offset == 0
            NvM_Date_t date;    // offset == 4
        } interpret;
    } data;

    NvM_validity_t validity;
} NvM_config_t;

NvM_config_t* NvM_GetConfig(void);
NvM_Status_e NvM_Read_All(void);
NvM_Status_e NvM_Save_Time(void);

#endif /* NvM_NvM_H_ */
