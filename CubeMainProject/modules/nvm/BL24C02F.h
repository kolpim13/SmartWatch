/*
 * BL24C02F.h
 *
 *  Created on: Jul 1, 2024
 *      Author: torgu
 * 
 * Page is 16 bytes long
 * Chip is able to get up to 8 bytes information in a row
 * So it is convinient to transfers data aligned up to 8 bytes (16 is better).
 * Entire chip memory is 256 bytes (2048 bits).
 * 
 * @todo:
 *  1. Add different options for read (sequentional and random access)
 *  2. Cover all driver related things into structure ==> Can be essential since up to 8 same chips can be set on 1 I2C.
 */

#ifndef NVM_BL24C02F_H_
#define NVM_BL24C02F_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#define BL24C02F_ADDRESS    0xA0 //0x50//0xA0
#define BL24C02F_MAX_LEN    256u

/**
 * @brief Function is needed in case when 
 * 
 */
void BL24C02F_ResetI2C(void);

/**
 * @brief 
 * 
 * @param data 
 * @param address 
 * @param len 
 */
void BL24C02F_WriteData(size_t address, uint8_t* data, size_t len);

/**
 * @brief Implement sequential read.
 * Assumes that all data always read, hence ==> read always will be done from the very begining.
 * 
 * @param data 
 * @param address 
 * @param len 
 */
void BL24C02F_ReadData(uint8_t* data, size_t len);

#endif /* NVM_BL24C02F_H_ */
