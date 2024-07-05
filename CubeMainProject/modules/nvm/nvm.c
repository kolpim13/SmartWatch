#include "nvm.h"

static NvM_config_t nvm_ram;

static NvM_Status_e NvM_Save(uint8_t address, uint8_t* data, size_t len);
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

NvM_config_t* NvM_GetConfig(void)
{
    return &nvm_ram;
}

NvM_Status_e NvM_Read_All(void)
{
    BL24C02F_Status_e stat = BL24C02F_ReadData((uint8_t)0u, (uint8_t *)&(nvm_ram.data.raw_bytes[0]), NvM_DATA_LEN);
    if (stat == BL24C02F_Status_I2C_Error)
    {
        return NvM_Status_Error;
    }

    return NvM_Status_OK;
}

NvM_Status_e NvM_Save_Time(void)
{
	return NvM_Save(0u, (uint8_t *)&(nvm_ram.data.interpret.time), sizeof(NvM_Time_t));
}
