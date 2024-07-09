/*
 * ST7789.c
 *
 *  Created on: Jun 24, 2024
 *      Author: torgu
 */

#include "ST7789.h"

extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_tx;
/*=================================================================*/

static void ST7789_WriteCommand(uint8_t command);
static void ST7789_WriteData(uint16_t data);
static void ST7789_WriteData8(uint8_t data);
static void ST7789_SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
/*=================================================================*/

static void ST7789_WriteCommand(uint8_t command)
{
    ST7789_DC_Command();
    HAL_SPI_Transmit(&hspi1, &command, 1, 1);
}

static void ST7789_WriteData(uint16_t data)
{
    uint8_t temp[2];
    temp[0] = (data >> 8) & 0xFF;
    temp[1] = data & 0xFF;

    ST7789_DC_Data();
    HAL_SPI_Transmit(&hspi1, &temp, 2, 1);
}

static void ST7789_WriteData8(uint8_t data)
{
    ST7789_DC_Data();
    HAL_SPI_Transmit(&hspi1, &data, 1, 1);
}

static void ST7789_SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    ST7789_WriteCommand(ST7789_CASET);
    ST7789_WriteData(x1);
    ST7789_WriteData(x2);

    ST7789_WriteCommand(ST7789_RASET);
    ST7789_WriteData(y1 + ST7789_OFFSET_Y);
    ST7789_WriteData(y2 + ST7789_OFFSET_Y);

    ST7789_WriteCommand(ST7789_RAMWR);
}
/*=================================================================*/

void ST7789_GPIO_Init(void)
{
    /* Init Pins for ST7789 driver. */
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    GPIO_InitStructure.Pin = (ST7789_RST_PIN | ST7789_CS_PIN | ST7789_DC_PIN | ST7789_BLK_PIN);
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Set all pins in its default state (Plus enable blacklusght). */
    HAL_GPIO_WritePin(GPIOA, (ST7789_RST_PIN | ST7789_DC_PIN | ST7789_BLK_PIN), GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, ST7789_CS_PIN, GPIO_PIN_RESET);

	/* Init DMA callback */

}

void ST7789_Init(void)
{
    /* hardware reset. */
    ST7789_RST_Clr();
    HAL_Delay(100);
    ST7789_RST_Set();
    HAL_Delay(100);

    /* Sleep out sequence. [Wait time can be decreesed to 5 ms according to datasheet] */
    ST7789_WriteCommand(0x11); 
    HAL_Delay(120);
	ST7789_WriteCommand(0x36);
    ST7789_WriteData8(0x00);

    ST7789_WriteCommand(ST7789_COLMOD);
	ST7789_WriteData8(0x05);

    ST7789_WriteCommand(0xB2);
	ST7789_WriteData8(0x0C);
	ST7789_WriteData8(0x0C);
	ST7789_WriteData8(0x00);
	ST7789_WriteData8(0x33);
	ST7789_WriteData8(0x33); 

    ST7789_WriteCommand(0xB7); 
	ST7789_WriteData8(0x35);  

	ST7789_WriteCommand(0xBB);
	ST7789_WriteData8(0x19);

	ST7789_WriteCommand(0xC0);
	ST7789_WriteData8(0x2C);

	ST7789_WriteCommand(0xC2);
	ST7789_WriteData8(0x01);

	ST7789_WriteCommand(0xC3);
	ST7789_WriteData8(0x12);   

	ST7789_WriteCommand(0xC4);
	ST7789_WriteData8(0x20);  

	ST7789_WriteCommand(0xC6); 
	ST7789_WriteData8(0x0F);    

	ST7789_WriteCommand(0xD0); 
	ST7789_WriteData8(0xA4);
	ST7789_WriteData8(0xA1);

	ST7789_WriteCommand(0xE0);
	ST7789_WriteData8(0xD0);
	ST7789_WriteData8(0x04);
	ST7789_WriteData8(0x0D);
	ST7789_WriteData8(0x11);
	ST7789_WriteData8(0x13);
	ST7789_WriteData8(0x2B);
	ST7789_WriteData8(0x3F);
	ST7789_WriteData8(0x54);
	ST7789_WriteData8(0x4C);
	ST7789_WriteData8(0x18);
	ST7789_WriteData8(0x0D);
	ST7789_WriteData8(0x0B);
	ST7789_WriteData8(0x1F);
	ST7789_WriteData8(0x23);

	ST7789_WriteCommand(0xE1);
	ST7789_WriteData8(0xD0);
	ST7789_WriteData8(0x04);
	ST7789_WriteData8(0x0C);
	ST7789_WriteData8(0x11);
	ST7789_WriteData8(0x13);
	ST7789_WriteData8(0x2C);
	ST7789_WriteData8(0x3F);
	ST7789_WriteData8(0x44);
	ST7789_WriteData8(0x51);
	ST7789_WriteData8(0x2F);
	ST7789_WriteData8(0x1F);
	ST7789_WriteData8(0x1F);
	ST7789_WriteData8(0x20);
	ST7789_WriteData8(0x23);

	ST7789_WriteCommand(ST7789_INVON); 

	ST7789_WriteCommand(ST7789_DISPON); 
}

void ST7789_FillArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t* color)
{
	ST7789_SetAddress(x1, y1, x2, y2);
    size_t size = (y2 - y1 + 1) * (x2 - x1 + 1);

    hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
	hspi1.Instance->CR1 |= SPI_CR1_DFF;

    ST7789_DC_Data();
    HAL_SPI_Transmit(&hspi1, (uint8_t *)color, size, HAL_MAX_DELAY);

    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Instance->CR1 &= ~SPI_CR1_DFF;
}

void ST7789_FillArea_Async(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t* color)
{
	ST7789_SetAddress(x1, y1, x2, y2);
    size_t size = (y2 - y1 + 1) * (x2 - x1 + 1);
	
	ST7789_PrepaerSendLargeData();

	ST7789_DC_Data();
	// if (hdma_spi1_tx.State == HAL_DMA_STATE_READY)
	// {
	// 	HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)color, size);
	// 	while(__HAL_DMA_GET_COUNTER(&hdma_spi1_tx) != 0);
	// }
	HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)color, size);
	while(__HAL_DMA_GET_COUNTER(&hdma_spi1_tx) != 0);
	//HAL_SPI_Transmit(&hspi1, (uint8_t*)color, size, 10);

	ST7789_PrepaerSendSmallData();
}

void ST7789_FillArea_PixelByPixel(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t* color)
{
    ST7789_SetAddress(x1, y1, x2, y2);
    size_t size = (y2 - y1) * (x2 - x1);

    for (size_t i = 0; i < size; i++)
    {
        ST7789_WriteData(color[i]);
    }
}

bool ST7789_Async_CheckTrasferFinished(void)
{
	return (__HAL_DMA_GET_COUNTER(&hdma_spi1_tx) == 0);
}

void ST7789_PrepaerSendSmallData(void)
{
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Instance->CR1 &= ~SPI_CR1_DFF;
}

void ST7789_PrepaerSendLargeData(void)
{
	hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
	hspi1.Instance->CR1 |= SPI_CR1_DFF;
}
