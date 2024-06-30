/**
 * @file CST816.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-06-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "CST816.h"

extern I2C_HandleTypeDef hi2c1;

/**
 * @brief 
 * 
 * @param address 
 * @param data 
 */
static void CST816_WriteReg(uint8_t address, uint8_t data);

/**
 * @brief 
 * 
 * @param address 
 * @return uint8_t 
 */
static void CST816_ReadRegs(uint8_t address, uint8_t* data, size_t len);

static void CST816_Config_AutoSleepTime(uint8_t time);
/*=================================================================*/

static void CST816_WriteReg(uint8_t address, uint8_t data)
{
    uint8_t temp[2] = {address, data};
    HAL_I2C_Master_Transmit(&hi2c1, CST816T_ADDRESS, temp, 2, HAL_MAX_DELAY);
}

static void CST816_ReadRegs(uint8_t address, uint8_t* data, size_t len)
{
    HAL_I2C_Master_Transmit(&hi2c1, CST816T_ADDRESS, &address, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(&hi2c1, CST816T_ADDRESS, data, len, HAL_MAX_DELAY);
}

static inline void CST816_Config_AutoSleepTime(uint8_t time_s)
{
    CST816_WriteReg(CST816_AUTO_SLEEP_TIME, time_s);
}
/*=================================================================*/

void CST816_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = CST816_RST_PORT;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(CST816_RST_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = CST816_INT_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(CST816_INT_PORT, &GPIO_InitStructure);
}

void CST816_Reset(void)
{
    CST816_RST_Clr();
	HAL_Delay(10);
	CST816_RST_Set();
	HAL_Delay(100);
}

bool CST816_IsTouch(void)
{
    if (HAL_GPIO_ReadPin(CST816_INT_PORT, CST816_INT_PIN) == GPIO_PIN_RESET)
    {
        return true;
    }
    return false;
}

void CST816_Init(void)
{
    CST816_GPIO_Init();
	CST816_Config_AutoSleepTime(5);
}

void CST816_GetAxis_XY(uint16_t* x, uint16_t* y)
{
    uint8_t data[4];
    CST816_ReadRegs(CST816_XPOS_H, data, 4);
	*x = ((data[0] & 0x0F) << 8) | data[1];
	*y = ((data[2] & 0x0F) << 8) | data[3];
}

uint8_t CST816_GetNumOfFingers(void)
{
    uint8_t data[1];
    CST816_ReadRegs(CST816_FINGER_NUM, data, 1);
    return data[0];
}
/*=================================================================*/
