/**
 * @file nvm.h
 * @author Maksym Torhunakov (torgunakovmaks2@gmail.com)
 * @brief 
 * @version 0.1 @date 2024-07-05
 *  - Initial release
 * 
 * @version 0.2 @date 2024-07-23
 *  - PP and CRC Mechanizmes add but NOT USED.
 * 
 * @version 0.3 @date 2024-11-23
 *  - Some descriptions to functions added
 *  - Interface NvM_GetConfig removed. Global variable is used instead. [Not yet | TBD]
 *  - Now CRC is used as a mechanizm of verification of data validity.
 * 
 * @todo Optimize time and complexity of the operations. {Create new module architecture} [HARD]
 * @todo Add function to save data to write to queue. (Read from the queue not needed now) [EASY]
 */

#ifndef NvM_NvM_H_
#define NvM_NvM_H_

#include "sys.h"
#include "BL24C02F.h"
#include "../RTC/RTC.h"
/*=================================================================*/

#define NvM_LEN         BL24C02F_MAX_LEN
#define NvM_LEN_DATA    (NvM_LEN - sizeof(NvM_Validity_t))

#define NvM_PP          (uint32_t)0x43211234    /* Right now is not used. */
/*=================================================================*/

/* ADDRESSES OF THE DATA BLOCKS */
#define NvM_BLOCK_RTC_ADR           (uint8_t)0
#define NvM_BLOCK_DISPLAY_ADR       (uint8_t)(NvM_BLOCK_RTC_ADR + sizeof(NvM_Block_RTC_t))
#define NvM_BLOCK_SYSTEM_ADR        (uint8_t)(NvM_BLOCK_DISPLAY_ADR + sizeof(NvM_Block_Display_t))
// #define NvM_BLOCK_... 
#define NvM_BLOCK_VALIDITY_ADR      (uint8_t)NvM_LEN_DATA
/*=================================================================*/

typedef enum 
{
    NvM_Status_OK = 0,
    NvM_Status_NotReady,
    NvM_Status_Error,
} NvM_Status_e;

typedef enum
{
    NvM_Block_RTC = (uint8_t)0,
    NvM_Block_Display,
    NvM_Block_System,
    NvM_Block_Force_U8 = (uint8_t)0xFFu
} NvM_Block_t;

typedef struct NvM_Validity_t
{
    uint32_t crc;
    uint32_t pp;    /* To store data about the system (SW / HW) */
} NvM_Validity_t;

typedef struct NvM_Block_RTC_t
{
    RTC_DateTypeDef Date;
    RTC_TimeTypeDef Time;
    RTC_TimeFormat_t time_format;
} NvM_Block_RTC_t;

typedef struct NvM_Block_Display_t
{
    uint8_t brightness_pc;
    uint8_t reserved[3];
} NvM_Block_Display_t;

typedef struct NvM_Block_System_t
{
    uint32_t time_to_low_power_ms;
} NvM_Block_System_t;

/**
 * @brief Structure should be 8 bytes aligned (As chip is 8-byte alligned).
 * 
 */
typedef struct NvM_Config_t
{
    union
    {
        uint8_t raw_bytes[NvM_LEN_DATA];
        struct
        {
            NvM_Block_RTC_t rtc;
            NvM_Block_Display_t display;
            NvM_Block_System_t system;
        };
    } data;

    NvM_Validity_t validity;
} NvM_Config_t;
/*=================================================================*/

extern NvM_Config_t nvm_ram;
/*=================================================================*/

/**
 * @brief Save all data blocks (but not validity block) into eeprom.
 * 
 * @return NvM_Status_e 
 */
NvM_Status_e NvM_Save_All(void);

/**
 * @brief Save data about RTC in Eeprom.
 * 
 * @return NvM_Status_OK - Operation successfull 
 *  NvM_Status_Error - Operation failed
 */
NvM_Status_e NvM_Save_RTC(void);

/**
 * @brief Save data about Display settings in Eeprom.
 * 
 * @return NvM_Status_OK - Operation successfull 
 *  NvM_Status_Error - Operation failed
 */
NvM_Status_e NvM_Save_Display(void);

/**
 * @brief 
 * 
 * @return NvM_Status_OK - Operation successfull 
 *  NvM_Status_Error - Operation failed
 */
NvM_Status_e NvM_Save_Validity(void);

/**
 * @brief 
 * 
 * @return NvM_Status_OK - Operation successfull 
 *  NvM_Status_Error - Operation failed
 */
NvM_Status_e NvM_Save_System(void);
/*=================================================================*/

/**
 * @brief This function should be ran on the startup of the ECU.
 * 
 * @return NvM_Status_e 
 */
NvM_Status_e NvM_StartUp(void);

/**
 * @brief Write default data into NvM RAM.
 * 
 */
void NvM_FillWithDefaultData(void);

/**
 * @brief Read all blocks (including validity).
 * 
 * @return NvM_Status_e 
 */
NvM_Status_e NvM_Read_All(void);

/**
 * @brief Calculate crc for data only that is stored in eeprom.
 * 
 * @return uint32_t 
 */
uint32_t NvM_CalculateCRC(void);
/*=================================================================*/

#endif /* NvM_NvM_H_ */
