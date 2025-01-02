#include "CST816.h"

typedef enum {
    IrqCtr_OnceWLP = 0x01,
    IrqCtr_EnMonitor = 0x10,
    IrqCtr_EnChange = 0x20,
    IrqCtr_EnTouch = 0x40,
    IrqCtr_EnTest = 0x80,
} IrqCtr_t;
/*=================================================================*/

/**
 * @brief Write single register
 * 
 * @param address 
 * @param data 
 */
static void WriteReg(uint8_t address, uint8_t data);

/**
 * @brief Read multiple registers
 * 
 * @param address First address
 * @param data Buffer to read to [out]
 * @param len Amount of byte to read
 */
static void ReadRegs(uint8_t address, uint8_t* data, size_t len);
/*=================================================================*/

static void WriteReg(uint8_t address, uint8_t data)
{
    uint8_t temp[2] = {address, data};
    HAL_I2C_Master_Transmit(&hi2c1, CST816T_ADDRESS, temp, 2, HAL_MAX_DELAY);
}

static void ReadRegs(uint8_t address, uint8_t* data, size_t len)
{
    HAL_I2C_Master_Transmit(&hi2c1, CST816T_ADDRESS, &address, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(&hi2c1, CST816T_ADDRESS, data, len, HAL_MAX_DELAY);
}
/*=================================================================*/

void CST816_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = CST816_RST_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(CST816_RST_PORT, &GPIO_InitStructure);
}

void CST816_Init(void)
{
    /* Enable chip. */
    CST816_RST_Set();
    HAL_Delay(50);

    /* Configure Chip */
    WriteReg(CST816_AUTO_SLEEP_TIME, 5);
    WriteReg(CST816_NOR_SCAN_PERIOD, 5);
    WriteReg(CST816_IRQ_CTL, (uint8_t)IrqCtr_EnChange);
}

void CST816_Reset_HW(void)
{
    CST816_RST_Reset();
	HAL_Delay(10);
	CST816_RST_Set();
	HAL_Delay(50);
}

bool CST816_IsTouch(void)
{
    uint8_t fingers = 0;
    ReadRegs(CST816_FINGER_NUM, &fingers, 1);
    
    /* If no finger detected --> no touch is present at the moment */
    if (fingers == 0)
    {
        return false;
    }
    return true;
}

void CST816_GetAxis_XY(uint16_t* x, uint16_t* y)
{
    uint8_t data[4];
    ReadRegs(CST816_XPOS_H, data, 4);
	*x = ((data[0] & 0x0F) << 8) | data[1];
	*y = ((data[2] & 0x0F) << 8) | data[3];
}

bool CST816_Touch_and_AxisXY(uint16_t* x, uint16_t* y)
{
    uint8_t data[5];

    /* finger number and positions are going as a sequence. */
    ReadRegs(CST816_FINGER_NUM, data, 5);

    /* No fingers detected --> Do not update coordinates. */
    if (data[0] == 0)
    {
        return false;
    }

    /* Update coordinates. */
    *x = ((data[1] & 0x0F) << 8) | data[2];
	*y = ((data[3] & 0x0F) << 8) | data[4];
    return true;
}

void CST816_Sleep_Enter(void)
{
    CST816_Reset_HW();
    WriteReg(CST816_POWER_MODE, 0x03);
}

void CST816_Sleep_Wakeup(void)
{
    /* This is a question */
    // ...
}
/*=================================================================*/
