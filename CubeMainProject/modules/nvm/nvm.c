#include "nvm.h"

NvM_Config_t nvm_ram;
/*=================================================================*/

/**
 * @brief 
 * 
 * @param address 
 * @param data 
 * @param len 
 * @return NvM_Status_e 
 */
static NvM_Status_e NvM_Save(uint8_t address, uint8_t* data, size_t len);
/*=================================================================*/

static inline NvM_Status_e NvM_Save(uint8_t address, uint8_t* data, size_t len)
{
    BL24C02F_Status_e stat = BL24C02F_WriteData(address, data, len);
    if (stat != BL24C02F_Status_OK)
    {
        if (stat == BL24C02F_Status_I2C_Error)
        {
            BL24C02F_ResetI2C();
            if (BL24C02F_WriteData(address, data, len) != BL24C02F_Status_OK)
            {
                return NvM_Status_Error;
            }
        }
        return NvM_Status_Error;
    }

    return NvM_Status_OK;
}

NvM_Status_e NvM_Save_All(void)
{
    return NvM_Save((uint8_t)0u, (uint8_t *)nvm_ram.data.raw_bytes, NvM_LEN);
}

NvM_Status_e NvM_Save_RTC(void)
{
	return NvM_Save(NvM_BLOCK_RTC_ADR, (uint8_t *)&(nvm_ram.data.rtc), sizeof(NvM_Block_RTC_t));
}

NvM_Status_e NvM_Save_Display(void)
{
    return NvM_Save(NvM_BLOCK_DISPLAY_ADR, (uint8_t *)&(nvm_ram.data.display), sizeof(NvM_Block_Display_t));
}

NvM_Status_e NvM_Save_Validity(void)
{
    return NvM_Save(NvM_BLOCK_VALIDITY_ADR, (uint8_t *)&(nvm_ram.validity), sizeof(NvM_Validity_t));
}
/*=================================================================*/

void NvM_FillWithDefaultData(void)
{
    /* Set date and time to "0" */
    memset((void *)&nvm_ram.data.rtc, 0, sizeof(NvM_Block_RTC_t));

    nvm_ram.data.display.brightness_pc = 100;
    memset((void *)nvm_ram.data.display.reserved, 0, sizeof(nvm_ram.data.display.reserved));
}

NvM_Status_e NvM_StartUp(void)
{
    NvM_Status_e status = NvM_Status_OK;

    /* Try to read data from eeprom 3 times in a row.*/
    for (size_t count = 0; count < 3; count++)
    {
        status = NvM_Read_All();
        if (NvM_Status_Error == status)
        {
            BL24C02F_ResetI2C();
        }
        else
        {
            break;
        }
    }

    /* If data was possible to read */
    uint32_t crc = 0;
    if (NvM_Status_OK == status)
    {
        /* And it has correct crc */
        crc = NvM_CalculateCRC();
        if (crc == nvm_ram.validity.crc)
        {
            /* Do nothing */
            return NvM_Status_OK;
        }
    }

    /* Otherwise --> write default data to the nvm_ram and eeprom with calculated crc. */
    nvm_ram.validity.crc = crc;
    NvM_FillWithDefaultData();
    NvM_Save_All();
    NvM_Save_Validity();
    /* Some checks on correctness previous actions should be added. */

    return NvM_Status_OK;
}

NvM_Status_e NvM_Read_All(void)
{
    BL24C02F_Status_e stat = BL24C02F_ReadData((uint8_t)0u, (uint8_t *)&(nvm_ram), NvM_LEN);
    if (stat == BL24C02F_Status_I2C_Error)
    {
        return NvM_Status_Error;
    }

    return NvM_Status_OK;
}

uint32_t NvM_CalculateCRC(void)
{
    return HAL_CRC_Calculate(&hcrc, (uint32_t *)(&nvm_ram.data.raw_bytes[0]), (NvM_LEN_DATA / 4));
}
/*=================================================================*/
