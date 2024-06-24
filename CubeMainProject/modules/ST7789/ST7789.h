/*
 * ST7789.h
 *
 *  Created on: Jun 24, 2024
 *      Author: torgu
 */

#ifndef ST7789_ST7789_H_
#define ST7789_ST7789_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

/* LCD physical parameters */
#define ST7789_LCD_WIDTH    240
#define ST7789_LCD_HEIGHT   280

/* Control Registers and constant codes */
#define ST7789_NOP     0x00
#define ST7789_SWRESET 0x01
#define ST7789_RDDID   0x04
#define ST7789_RDDST   0x09

#define ST7789_SLPIN   0x10     // Enter sleep mode
#define ST7789_SLPOUT  0x11     // Exit sleep mode
#define ST7789_PTLON   0x12
#define ST7789_NORON   0x13

#define ST7789_INVOFF  0x20
#define ST7789_INVON   0x21
#define ST7789_DISPOFF 0x28     // Disable display: screen does not updated from memory
#define ST7789_DISPON  0x29     // Enable display
#define ST7789_CASET   0x2A     // Column address set
#define ST7789_RASET   0x2B     // Row address set 
#define ST7789_RAMWR   0x2C     // Memory write
#define ST7789_RAMRD   0x2E     // Memory read

#define ST7789_PTLAR   0x30     // 
#define ST7789_COLMOD  0x3A     // Interface pixel format
#define ST7789_MADCTL  0x36

#define ST7789_OFFSET_Y 20      // Offset in HW
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

static void ST7789_WriteCommand(uint8_t command);
static void ST7789_WriteData(uint16_t data);
static void ST7789_WriteData8(uint8_t data);
static void ST7789_SetAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
/*=================================================================*/

void ST7789_GPIO_Init(void);
void ST7789_Init(void);
void ST7789_FillArea(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t* color);
void ST7789_FillAreaWithColor(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
/*=================================================================*/

#endif /* ST7789_ST7789_H_ */
