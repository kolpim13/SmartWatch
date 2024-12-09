#include "cli.h"

volatile bool cli_transmition_active = false;
/*=================================================================*/

// static CLI_INTERFACE_TYPE cli_buffer_send[CLI_BUFFER_SIZE_SEND];
// static CLI_INTERFACE_TYPE cli_buffer_receive[CLI_BUFFER_SIZE_RECEIVE];
/*=================================================================*/

CLI_Status_e CLI_Send_Sync(CLI_INTERFACE_TYPE* data, size_t len)
{
    if (cli_transmition_active == true) {return CLI_Status_BUSY;}

    cli_transmition_active = true;
    HAL_StatusTypeDef ret = HAL_UART_Transmit(&UART_CLI, (const uint8_t *)data, (uint16_t)len, 1);
    cli_transmition_active = false;

    if (ret == HAL_ERROR) { return CLI_Status_Error; }
    if (ret == HAL_BUSY) { return CLI_Status_BUSY; }
    if (ret == HAL_TIMEOUT) { return CLI_Status_TIMEOUT; }
    return CLI_Status_OK;
}
CLI_Status_e CLI_Send_Async_Static(CLI_INTERFACE_TYPE* data, size_t len)
{
    /* If there is no transaction at the moment --> Send right now. */
    if (cli_transmition_active == false)
    {
        HAL_StatusTypeDef ret = HAL_UART_Transmit_DMA(&UART_CLI, (const uint8_t *)data, (uint16_t)len);

        if (ret == HAL_ERROR) { return CLI_Status_Error; }
        if (ret == HAL_BUSY) { return CLI_Status_BUSY; }
        if (ret == HAL_TIMEOUT) { return CLI_Status_TIMEOUT; }
        return CLI_Status_OK;
    }

    /* Otherwise --> queue message to send. */
    CLI_Data_Async_Static_t send_data_async_static = { .data = data, .len = len };
    if (xQueueSendToBack
    (
        CliQueueHandle,
        (void *)&send_data_async_static,
        (TickType_t)0 
    ) != pdPASS)
    {
        return CLI_Status_BUSY;
    }

    return CLI_Status_OK;
}
CLI_Status_e CLI_Send_Async_Dynamic(CLI_INTERFACE_TYPE* data, size_t len)
{
    (void) len;
    return CLI_Status_OK;
}
/*=================================================================*/

void CLI_Main_Cyclic(void)
{
    // Process reception 

    /* Read and process all Static messages to be sent */
    CLI_Data_Async_Static_t send_data_async_static;
    if (xQueueReceive
    (
        CliQueueHandle,
        &send_data_async_static,
        (TickType_t)0
    ) == pdPASS)
    {
        cli_transmition_active = true;
        HAL_UART_Transmit_DMA(&UART_CLI, (const uint8_t *)send_data_async_static.data, (uint16_t)send_data_async_static.len);
    }

    /* Read and process all Dynamic data to be sent */
}
/*=================================================================*/
