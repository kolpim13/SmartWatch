/**
 * @file cli.h
 * @author Maksym Torhunakov (torgunakovmaks2@gmail.com)
 * @brief 
 * @version 0.1 @date 2024-11-16 
 *  - Work bounded to the UART and it`s HAL implementation
 *  - Can only send data through UART | Receive was not implemented yet.
 *  - Sync and Async methods can not be used simultaneousely for the same HW interface.
 *  - Was not tested on the real HW.
 *
 * 
 * @copyright Copyright (c) 2024
 * 
 * @todo V0.1 to be tested on the real HW!!!
 * @todo All logic from the callback in [sys.c] file to be transfered to function in [cli.c] file
 * @todo Make main functions more abstract. So it can be ported on any other platform more easely | or other HW interfaces used.
 * @todo Add support of possible interfaces to communicate.
 *      Should be defined during compilation time. [Affect: cli_config.h]
 */

#ifndef CLI_H
#define CLI_H

#include "sys.h"
#include "cli_config.h"
/*=================================================================*/

extern volatile bool cli_transmition_active;
/*=================================================================*/

typedef enum
{
    CLI_Status_OK = 0,
    CLI_Status_Error,
    CLI_Status_BUSY,
    CLI_Status_TIMEOUT,
}CLI_Status_e;
/*=================================================================*/

typedef struct
{
    CLI_INTERFACE_TYPE* data;
    size_t len;
} CLI_Data_Async_Static_t;
/*=================================================================*/

/**
 * @brief Sends data using chosen UART in blocking mode
 * 
 * @param data Pointer on data to be sent 
 * @param len  Length of the data in [CLI_INTERFACE_TYPE] defined in [cli_config.h]
 * @return CLI_Status_e 
 */
CLI_Status_e CLI_Send_Sync(CLI_INTERFACE_TYPE* data, size_t len);

/**
 * @brief To send static [from ROM] data. 
 * If there is no transaction at the moment - sends data at the same moment.
 * If not - queue them, so it will be transfered later.
 * 
 * @param data Pointer on data to be sent
 * @param len  Length of the data in [CLI_INTERFACE_TYPE] defined in [cli_config.h]
 * @return CLI_Status_e 
 */
CLI_Status_e CLI_Send_Async_Static(CLI_INTERFACE_TYPE* data, size_t len);

CLI_Status_e CLI_Send_Async_Dynamic(CLI_INTERFACE_TYPE* data, size_t len);
/*=================================================================*/

/**
 * @brief 
 * 
 */
void CLI_Main_Cyclic(void);
/*=================================================================*/

#endif //CLI_H
