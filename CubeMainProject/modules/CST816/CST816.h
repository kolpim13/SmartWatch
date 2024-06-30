/**
 * @file CST816.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-06-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stdbool.h"

/* Address itself is 0x15.
 * But it assumes to be 0x2A for write and 0x2B for read operations.
 * Hal will add "1" to the address for read operations automatically. */
#define CST816T_ADDRESS         0x2A//0x15

#define CST816_OFFSET_X			20u
#define CST816_OFFSET_Y			20u

#define CST816_GESTURE_ID       0x01
#define CST816_FINGER_NUM       0x02
#define CST816_XPOS_H           0x03
#define CST816_XPOS_L           0x04
#define CST816_YPOS_H           0x05
#define CST816_YPOS_L           0x06
#define CST816_CHIP_ID          0xA7
#define CST816_PROJ_ID          0xA8
#define CST816_FW_VERSION       0xA9
#define CST816_FACTORY_ID       0xAA
#define CST816_SLEEP_MODE       0xE5
#define CST816_AUTO_SLEEP_TIME  0xF9
#define CST816_IRQ_CTL          0xFA
#define CST816_LONG_PRESS_TICK  0xEB
#define CST816_MOTION_MASK      0xEC
#define CST816_DIS_AUTOSLEEP    0xFE
/*=================================================================*/

#define CST816_RST_PORT         GPIOB
#define CST816_RST_PIN          GPIO_PIN_4

#define CST816_INT_PORT         GPIOA
#define CST816_INT_PIN          GPIO_PIN_15
/*=================================================================*/

#define CST816_RST_Set()        HAL_GPIO_WritePin(CST816_RST_PORT, CST816_RST_PIN, GPIO_PIN_SET)
#define CST816_RST_Clr()        HAL_GPIO_WritePin(CST816_RST_PORT, CST816_RST_PIN, GPIO_PIN_RESET)
/*=================================================================*/

void CST816_GPIO_Init(void);
void CST816_Reset(void);
void CST816_Init(void);
void CST816_GetAxis_XY(uint16_t* x, uint16_t* y);

uint8_t CST816_GetNumOfFingers(void);

bool CST816_IsTouch(void);

/**
 * @brief TBD
 * 
 */
void CST816_Wakeup(void);

/**
 * @brief TBD
 * 
 */
void CST816_Sleep(void);
