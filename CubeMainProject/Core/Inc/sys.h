/**
 * @file sys.h
 * @author Maksym T. (torgunakovmaks2@gmail.com)
 * @brief This module manages all standart includes, shared resources for the entire project.
 * In addiction it should provide implementations for the system callbacks interrupt [such as timers, dma, communication, etc.].
 * @version 0.1 @date 2024-07-11
 * @version 0.2 @date 2024-11-22
 *  - HW Version added
 * @version 0.3 @date 2024-12-05
 *  - Interface "SYS_Build_Init" added.
 *  Serves to configure SW depending on HW version and/or build type.
 * 
 * 
 * 
 * @todo 
 * 1. Add function to get absolut value of time passed since processor was enabled
 *      (in ms.)[Will be used by lvgl for tracking time for update purposes].
 * 2. Utilize defines for enabling and disabling modules for those wich can be en/dis.
 * 3. En/Dis modules depending on HW version and/or SW Build version.
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

/* HW VERSION */
#define HW_V1_0             100
#define HW_V1_1             101
#define HW_V1_2             102

#define HW_VERSION          HW_V1_0

/* DEBUG OR SERIAL SW */
#define SW_BUILD_DEBUG      0
#define SW_BUILD_SERIAL     1

#define SW_BUILD            SW_BUILD_DEBUG

/* ENABLE OR DISABLE SOME MODULES */
#define MODULE_DISABLE      0
#define MODULE_ENABLE       1

#define CLI_MODULE_ENABLE          MODULE_ENABLE
#define DEBUGPINS_MODULE_ENABLE    MODULE_ENABLE
/*=================================================================*/

/* SHARED RESOURCES AMOUNG PROJECT */
extern RTC_HandleTypeDef hrtc;
extern CRC_HandleTypeDef hcrc;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;
extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;

extern QueueHandle_t CliQueueHandle;
/*=================================================================*/

/* DEFINE RESOURCES OF THE MACRO */
#define TIM_PWR         htim10
#define TIM_RTC         htim11
#define UART_CLI        huart1
/*=================================================================*/

/* INTERRUPT CALLBACKS */
// ...
/*=================================================================*/

/**
 * @brief Enable / disable features depending on specific HW and/or SW build type
 * 
 */
void SYS_Build_Init(void);
/*=================================================================*/


#endif //CORE_SYS_H_
