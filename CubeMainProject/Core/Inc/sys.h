/**
 * @file sys.h
 * @author Maksym T. (torgunakovmaks2@gmail.com)
 * @brief 
 * @version 0.1 @date 2024-07-11
 * 
 * 
 * 
 * @copyright Copyright (c) 2024
 * @todo 
 * 1. Add function to get absolut value of time passed since processor was enabled
 *      (in ms.)[Will be used by lvgl for tracking time for update purposes].
 * 2.
 */

#ifndef CORE_SYS_H_
#define CORE_SYS_H_

#include <stdint.h>
#include <stdbool.h>

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#include <FreeRTOS.h>
#include <queue.h>
/*=================================================================*/

/* SHARED RESOURCES AMOUNG PROJECT */
extern RTC_HandleTypeDef hrtc;
extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern TIM_HandleTypeDef htim3;
extern CRC_HandleTypeDef hcrc;
extern UART_HandleTypeDef huart1;

extern QueueHandle_t CliQueueHandle;
/*=================================================================*/

/* DEFINE RESOURCES OF THE MACRO */
#define UART_CLI        huart1
/*=================================================================*/

/* INTERRUPT CALLBACKS */
// ...
/*=================================================================*/


#endif //CORE_SYS_H_
