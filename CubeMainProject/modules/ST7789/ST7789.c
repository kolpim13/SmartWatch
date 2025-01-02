/*
 * ST7789.c
 *
 *  Created on: Jun 24, 2024
 *      Author: torgu
 */

#include "ST7789.h"
#include "nvm.h"
/*=================================================================*/

/* CONTROL REGISTERS */

#define NOP         0x00
#define SWRESET     0x01
#define RDDID       0x04
#define RDDST       0x09

#define SLPIN       0x10     // Enter sleep mode
#define SLPOUT      0x11     // Exit sleep mode
#define PTLON       0x12
#define NORON       0x13

#define INVOFF      0x20    // Display Inversion Off
#define INVON       0x21    // Display Inversion On 
#define DISPOFF     0x28    // Disable display: screen does not updated from memory
#define DISPON      0x29    // Enable display
#define CASET       0x2A    // Column address set
#define RASET       0x2B    // Row address set 
#define RAMWR       0x2C    // Memory write
#define RAMRD       0x2E    // Memory read

#define PTLAR       0x30    // Partial Area
#define COLMOD      0x3A    // Interface pixel format
#define MADCTL      0x36    // Memory Data Access Control

/* Leave those */
#define PORCTRL	    0xB2    // Porch Setting (leave default values)
#define GCTRL       0xB7    // Gate Control (leave default values)
#define VCOMS       0xBB    // Setting limitation: VCOMS + VCOMS offset + VDV = 0.1V~1.675V.
#define LCMCTRL     0xC0
#define IDSET       0xC1    // ID Code Setting
#define VDVVRHEN    0xC2    // VDV and VRH Command Enable 
#define VRHS        0xC3    // VRH Set
#define VDVS        0xC4    // VDV Set
#define VCMOFSET    0xC5    // VCOMS Offset Set
#define FRCTRL2     0xC6    // Frame Rate Control in Normal Mode 
#define PWCTRL1     0xD0    // Power Control 1 
#define PVGAMCTRL   0xE0    // Positive Voltage Gamma Control 
#define NVGAMCTRL   0xE1    // Negative Voltage Gamma Control 
/*=================================================================*/

static void WriteCommand(uint8_t command);
static void WriteData(uint16_t data);
static void WriteData8(uint8_t data);
static void SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
static void PrepaerSendSmallData(void);
static void PrepaerSendLargeData(void);
/*=================================================================*/

static volatile bool data_transfer_finished = true;

/* To be used in the future */
static uint16_t blacklight_raw = 0;     /* [0 - 300] */
static uint16_t blacklight_pc = 0;      /* [0 - 100] */
/*=================================================================*/

static inline void WriteCommand(uint8_t command)
{
    ST7789_DC_Command();
    HAL_SPI_Transmit(&hspi1, &command, 1, 1);
}

static inline void WriteData(uint16_t data)
{
    uint8_t temp[2];
    temp[0] = (data >> 8) & 0xFF;
    temp[1] = data & 0xFF;

    ST7789_DC_Data();
    HAL_SPI_Transmit(&hspi1, &temp, 2, 1);
}

static inline void WriteData8(uint8_t data)
{
    ST7789_DC_Data();
    HAL_SPI_Transmit(&hspi1, &data, 1, 1);
}

static inline void WriteDataBuffer(uint8_t* data)
{
    ST7789_DC_Data();
    HAL_SPI_Transmit(&hspi1, data, sizeof(data) - 1, 1);
}

static void SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    WriteCommand(CASET);
    WriteData(x1);
    WriteData(x2);

    WriteCommand(RASET);
    WriteData(y1 + ST7789_OFFSET_Y);
    WriteData(y2 + ST7789_OFFSET_Y);

    WriteCommand(RAMWR);
}

static inline void PrepaerSendSmallData(void)
{
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Instance->CR1 &= ~SPI_CR1_DFF;
}

static inline void PrepaerSendLargeData(void)
{
	hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
	hspi1.Instance->CR1 |= SPI_CR1_DFF;
}
/*=================================================================*/

void ST7789_GPIO_Init(void)
{
    /* Init Pins for ST7789 driver. */
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    GPIO_InitStructure.Pin = (ST7789_RST_PIN | ST7789_CS_PIN | ST7789_DC_PIN);
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Start PWM */
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

    /* Set all pins in its default state + enable blacklusght). */
    HAL_GPIO_WritePin(GPIOA, (ST7789_RST_PIN | ST7789_DC_PIN), GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, ST7789_CS_PIN, GPIO_PIN_RESET);
}

void ST7789_Init(void)
{
    /* Hardware reset. */
    ST7789_RST_Clr();
    HAL_Delay(10);
    ST7789_RST_Set();
    HAL_Delay(10);

    WriteCommand(SLPOUT); 
    HAL_Delay(5);
	WriteCommand(MADCTL);
    WriteData8(0x00);

    WriteCommand(COLMOD);
	WriteData8(0x05);

    WriteCommand(PORCTRL);
    WriteDataBuffer("\x0C\x0C\x00\x33\x33"); 

    WriteCommand(GCTRL); 
	WriteData8(0x35);  

	WriteCommand(VCOMS);
	WriteData8(0x19);

	WriteCommand(LCMCTRL);
	WriteData8(0x2C);

	WriteCommand(VDVVRHEN);
	WriteData8(0x01);

	WriteCommand(VRHS);
	WriteData8(0x12);   

	WriteCommand(VDVS);
	WriteData8(0x20);  

	WriteCommand(FRCTRL2); 
	WriteData8(0x0F);    

	WriteCommand(PWCTRL1); 
	WriteData8(0xA4);
	WriteData8(0xA1);

	WriteCommand(PVGAMCTRL);
    WriteDataBuffer("\xD0\x04\x0D\x11\x13\x2B\x3F\x54\x4C\x18\x0D\x0B\x1F\x23");

	WriteCommand(NVGAMCTRL);
    WriteDataBuffer("\xD0\x04\x0C\x11\x13\x2C\x3F\x44\x51\x2F\x1F\x1F\x20\x23");

	WriteCommand(INVON); 

	WriteCommand(DISPON);

	ST7789_SetLight(nvm_ram.data.display.brightness_pc);
}

void ST7789_SleepIn(void)
{
	WriteCommand(SLPIN);
	HAL_Delay(5);

    /* Disable blacklight */
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
}

void ST7789_SleepOut(void)
{
	WriteCommand(SLPOUT);
	HAL_Delay(5);

    /* Enable blacklight */
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
}

void ST7789_FillArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t* color)
{
	if (data_transfer_finished == false)
	{
		return;
	}
	data_transfer_finished = false;

	SetAddress(x1, y1, x2, y2);
    size_t size = (y2 - y1 + 1) * (x2 - x1 + 1);

	PrepaerSendLargeData();
    ST7789_DC_Data();
    HAL_SPI_Transmit(&hspi1, (uint8_t *)color, size, HAL_MAX_DELAY);
    PrepaerSendSmallData();

	data_transfer_finished = true;
}

void ST7789_FillArea_Async(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t* color)
{
	/* If data transfer already started ==> return */
	if (data_transfer_finished == false)
	{
		return;
	}
	data_transfer_finished = false;

	SetAddress(x1, y1, x2, y2);
    size_t size = (y2 - y1 + 1) * (x2 - x1 + 1);
	
	PrepaerSendLargeData();

	ST7789_DC_Data();
	HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)color, size);
}

void ST7789_SetLight(uint8_t light)
{
    if (light < 5) { light = 5; } 
    if (light > 100) { light = 100; }

    blacklight_pc = light;
    blacklight_raw = blacklight_pc * (ST7789_BLK_PWM_PERIOD / 100);

    __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, blacklight_raw);
}

uint8_t ST7789_GetLight(void)
{
	return blacklight_pc;
}

bool ST7789_CheckTrasferFinished(void)
{
	return data_transfer_finished;
}

void ST7789_Async_TxFinished_cb(void)
{
	/* After transfering data through DMA usual data format should be restored */
	PrepaerSendSmallData();
	data_transfer_finished = true;
}
