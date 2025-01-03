/**
 * @file ST7789.h
 * @author Maksym T. (torgunakovmaks2@gmail.com)
 * @brief LCD TFT low level driver.
 * @version 0.1 @date 2024-06-27
 *  Initial release
 * 
 * @version 0.2:
 *  Now supports asyncronous data transfer using DMA.
 * 
 * @version 0.3:
 *  - Control BlackLight pin over PWM (brightness control added).
 * 
 * @version 0.4:
 *  - Interface for reading current blacklight state added. 
 * 
 * @version 0.5 @date 2023-12-28:
 *  - Naming for static functions and defines changed - No module name present.
 *  - Implementation for several commands added:
 *      * Sleep In
 *      * Sleep Out
 *      * Etc.
 *  - 
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef ST7789_ST7789_H_
#define ST7789_ST7789_H_

#include "sys.h"

/* LCD physical parameters */
#define ST7789_LCD_WIDTH    240
#define ST7789_LCD_HEIGHT   280

#define ST7789_OFFSET_Y 	20      // Offset in HW
/*=================================================================*/

#define ST7789_BLK_PWM_PERIOD   300
/*=================================================================*/

#define ST7789_CLK_PORT         GPIOB
#define ST7789_CLK_PIN          GPIO_PIN_3

#define ST7789_MOSI_PORT        GPIOB
#define ST7789_MOSI_PIN         GPIO_PIN_5

#define ST7789_CS_PORT          GPIOA
#define ST7789_CS_PIN           GPIO_PIN_1

#define ST7789_RST_PORT         GPIOA
#define ST7789_RST_PIN          GPIO_PIN_0

#define ST7789_DC_PORT          GPIOA
#define ST7789_DC_PIN           GPIO_PIN_2

#define ST7789_BLK_PORT         GPIOA
#define ST7789_BLK_PIN          GPIO_PIN_6
/*=================================================================*/

#define ST7789_CS_Select()      HAL_GPIO_WritePin(ST7789_CS_PORT, ST7789_CS_PIN, GPIO_PIN_RESET)
#define ST7789_CS_Deselect()    HAL_GPIO_WritePin(ST7789_CS_PORT, ST7789_CS_PIN, GPIO_PIN_SET)

#define ST7789_DC_Command()     HAL_GPIO_WritePin(ST7789_DC_PORT, ST7789_DC_PIN, GPIO_PIN_RESET)
#define ST7789_DC_Data()        HAL_GPIO_WritePin(ST7789_DC_PORT, ST7789_DC_PIN, GPIO_PIN_SET)

#define ST7789_BLK_En()         HAL_GPIO_WritePin(ST7789_BLK_PORT, ST7789_BLK_PIN, GPIO_PIN_SET)
#define ST7789_BLK_Dis()        HAL_GPIO_WritePin(ST7789_BLK_PORT, ST7789_BLK_PIN, GPIO_PIN_RESET)

#define ST7789_RST_Set()        HAL_GPIO_WritePin(ST7789_RST_PORT, ST7789_RST_PIN, GPIO_PIN_SET)
#define ST7789_RST_Clr()        HAL_GPIO_WritePin(ST7789_RST_PORT, ST7789_RST_PIN, GPIO_PIN_RESET)
/*=================================================================*/

void ST7789_GPIO_Init(void);

/**
 * @brief 
 * 
 * @param x1 
 * @param y1 
 * @param x2 
 * @param y2 
 * @param color 
 */
void ST7789_FillArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t* color);

/**
 * @brief 
 * 
 * @param x1 
 * @param y1 
 * @param x2 
 * @param y2 
 * @param color 
 */
void ST7789_FillArea_Async(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t* color);

/**
 * @brief Set Brightness of the LCD screen
 * 
 * @param light - brightness in percentage [5 - 100] 
 */
void ST7789_SetLight(uint8_t light);

/**
 * @brief Get Current Brightnees of the LCD screen
 * 
 * @return uint8_t - light in percentage [0 - 100] 
 */
uint8_t ST7789_GetLight(void);

/**
 * @brief Check if ST7789 is BUSY transferring data.
 * 
 * @return true - ST7789 is ready to transfer new data 
 * @return false - ST7789 is BUSY at the moment 
 */
bool ST7789_CheckTrasferFinished(void);

/**
 * @brief Should be called from external modules to notify that HW data can be transfered again.
 * 
 */
void ST7789_Async_TxFinished_cb(void);
/*=================================================================*/

void ST7789_Init(void);
void ST7789_SleepIn(void);
void ST7789_SleepOut(void);
/*=================================================================*/

#endif /* ST7789_ST7789_H_ */
