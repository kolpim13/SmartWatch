/*
 * BL24C02F.c
 *
 *  Created on: Jul 1, 2024
 *      Author: torgu
 */


#include "BL24C02F.h"

extern I2C_HandleTypeDef hi2c2;

void BL24C02F_ResetI2C(void)
{
	HAL_I2C_DeInit(&hi2c2);

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
    for (int i = 0; i < 21; i++) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
        HAL_Delay(20);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_Delay(20);
    }

    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    hi2c2.Instance = I2C2;
    hi2c2.Init.ClockSpeed = 400000;
    hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c2.Init.OwnAddress1 = 0;
    hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c2.Init.OwnAddress2 = 0;
    hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&hi2c2);
}

void BL24C02F_WriteData(size_t address, uint8_t* data, size_t len)
{
    volatile HAL_StatusTypeDef ret;

    /* Check if data can be written into the eeprom. */
    if ((address + len) > BL24C02F_MAX_LEN)
    {
        /* Add return code */
        // ...
        return;
    }
    
    ret = HAL_I2C_Mem_Write(&hi2c2, BL24C02F_ADDRESS, address, I2C_MEMADD_SIZE_8BIT, data, len, HAL_MAX_DELAY);
    //ret = HAL_I2C_Master_Transmit(&hi2c2, BL24C02F_ADDRESS, data, len, HAL_MAX_DELAY);
    if (ret != HAL_OK)
    {
        BL24C02F_ResetI2C();
        ret = HAL_I2C_Mem_Write(&hi2c2, BL24C02F_ADDRESS, address, I2C_MEMADD_SIZE_8BIT, data, len, HAL_MAX_DELAY);
        //ret = HAL_I2C_Master_Transmit(&hi2c2, BL24C02F_ADDRESS, data, len, HAL_MAX_DELAY);
    }
}

void BL24C02F_ReadData(uint8_t* data, size_t len)
{
    volatile HAL_StatusTypeDef ret;

    //volatile HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(&hi2c2, BL24C02F_ADDRESS, 0, I2C_MEMADD_SIZE_8BIT, data, 256, 10);
    ret = HAL_I2C_Master_Receive(&hi2c2, BL24C02F_ADDRESS, data, len, HAL_MAX_DELAY);

    if (ret != HAL_OK)
    {
    	BL24C02F_ResetI2C();
    	ret = HAL_I2C_Master_Receive(&hi2c2, BL24C02F_ADDRESS, data, len, HAL_MAX_DELAY);
    }

}
