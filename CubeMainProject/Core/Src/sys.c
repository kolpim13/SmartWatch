#include "sys.h"

#include "ST7789.h"

#include "cli.h"

#include "lvgl/lvgl.h"
#include "lvgl/lv_port_disp.h"
#include "lvgl/lv_port_indev.h"
/*=================================================================*/

/* INTERRUPT CALLBACKS [HAL] */

/**
 * @brief 
 * 
 * @param hspi SPI handler instance
 */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    /* LCD DMA Callback */
    if (hspi == &hspi1)
    {
        disp_flush_ready_cb();
    }
}

/**
 * @brief 
 * 
 * @param huart UART hanlder instance
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    /* CLI DMA Callback */
    if (&UART_CLI == huart)
    {
        /* Try to extract next message. Success --> initiate next send. */
        CLI_Data_Async_Static_t send_data_async_static;
        if (xQueueReceiveFromISR 
        (
            CliQueueHandle,
            (void *)&send_data_async_static,
            NULL
        ) == pdPASS)
        {
            HAL_UART_Transmit_DMA(&UART_CLI, (const uint8_t *)send_data_async_static.data, (uint16_t)send_data_async_static.len);
        }

        /* No items in the queue --> inform cli module that it is free. */
        else
        {
            cli_transmition_active = false;
        }
    }
}
/*=================================================================*/

void SYS_Build_Init(void)
{
    #if SW_BUILD == SW_BUILD_DEBUG
    DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;		/* IWDG is stopped when main core is halted. */
    #else
    return;
    #endif
}
/*=================================================================*/
