/**
 * @file cli_config.h
 * @author Maksym Torhunakov (torgunakovmaks2@gmail.com)
 * @brief 
 * @version 0.1 @date 2024-11-16
 * 
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef CLI_CONFIG_H
#define CLI_CONFIG_H

#include "stdint.h"

#define CLI_USE_SEND_BUFFER         0
#define CLI_USE_RECEIVE_BUFFER      0

#define CLI_INTERFACE_TYPE          uint8_t
#define CLI_BUFFER_SIZE_SEND        512
#define CLI_BUFFER_SIZE_RECEIVE     512

#define CLI_SYNC_TIMEOUT_MS         10
#define CLI_QUEUE_SIZE              (size_t)5       /* Right now does not used. Parameter configured in Cube IDE. */

#endif //CLI_CONFIG_H
