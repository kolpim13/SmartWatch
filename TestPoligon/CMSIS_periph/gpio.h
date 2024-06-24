/*
 * gpio.h
 *
 *  Created on: Jun 15, 2024
 *      Author: torgu
 */

#ifndef PERIPHERALS_GPIO_H_
#define PERIPHERALS_GPIO_H_

#include "stm32f411xe.h"
#include "stdint.h"

typedef enum GPIO_PIN_e
{
	GPIO_PIN_0_POS          = 0U,
	GPIO_PIN_1_POS          = 1U,
	GPIO_PIN_2_POS          = 2U,
	GPIO_PIN_3_POS          = 3U,
	GPIO_PIN_4_POS          = 4U,
	GPIO_PIN_5_POS          = 5U,
	GPIO_PIN_6_POS          = 6U,
	GPIO_PIN_7_POS          = 7U,
    GPIO_PIN_8_POS          = 8U,
	GPIO_PIN_9_POS          = 9U,
	GPIO_PIN_10_POS         = 10U,
	GPIO_PIN_11_POS         = 11U,
	GPIO_PIN_12_POS         = 12U,
	GPIO_PIN_13_POS         = 13U,
	GPIO_PIN_14_POS         = 14U,
	GPIO_PIN_15_POS         = 15U,
} GPIO_PIN_e;

typedef enum GPIO_MODER_e
{
    GPIO_MODER_INPUT        = 0U,
    GPIO_MODER_OUTPUT       = 1U,
    GPIO_MODER_ALTERNATE    = 2U,
    GPIO_MODER_ANALOG       = 3U,
    GPIO_MODER_FORCE_U32    = 0xFFFFFFFFU,
} GPIO_MODER_e;

typedef enum GPIO_OTYPER_e
{
    GPIO_OTYPER_PUSHPULL    = 0U,
    GPIO_OTYPER_OPENDRAIN   = 1U,
    GPIO_OTYPER_FORCE_U16   = 0xFFFFU,
} GPIO_OTYPER_e;

/* Commented due to fact these definitins are present in HAL library
typedef enum GPIO_SPEED_e
{
    GPIO_SPEED_LOW          = 0U,
    GPIO_SPEED_MEDIUM       = 1U,
    GPIO_SPEED_FAST         = 2U,
    GPIO_SPEED_HIGH         = 3U,
    GPIO_SPEED_FORCE_U32    = 0xFFFFFFFFU,
} GPIO_SPEED_e;
*/

typedef enum GPIO_PUPDR_e
{
    GPIO_PUPDR_NO           = 0U,
    GPIO_PUPDR_UP           = 1U,
    GPIO_PUPDR_DOWN         = 2U,
    GPIO_PUPDR_RESERVED     = 3U,
    GPIO_PUPDR_FORCE_U32    = 0xFFFFFFFFU,
} GPIO_PUPDR_e;

typedef enum GPIO_AFR_e
{
	GPIO_AFR_AF0            = 0U,
	GPIO_AFR_AF1            = 1U,
	GPIO_AFR_AF2            = 2U,
	GPIO_AFR_AF3            = 3U,
    GPIO_AFR_AF4            = 4U,
	GPIO_AFR_AF5            = 5U,
	GPIO_AFR_AF6            = 6U,
	GPIO_AFR_AF7            = 7U,
    GPIO_AFR_AF8            = 8U,
	GPIO_AFR_AF9            = 9U,
	GPIO_AFR_AF10           = 10U,
	GPIO_AFR_AF11           = 11U,
    GPIO_AFR_AF12           = 12U,
	GPIO_AFR_AF13           = 13U,
	GPIO_AFR_AF14           = 14U,
	GPIO_AFR_AF15           = 15U,
} GPIO_AFR_e;

/**
 * @brief 
 * 
 */
/*
#define GPIO_CONFIGURE(PORT, PIN, MODER, OTYPER, SPEED, PUPDR, ALTER) {\
    PORT->MODER &= ~(MODER << (PIN * 2)); \
    PORT->MODER |= (MODER << (PIN * 2)); \
    PORT->OTYPER &= ~(OTYPER << PIN); \
    PORT->OTYPER |= (OTYPER << PIN); \
    PORT->OSPEEDR &= ~(SPEED << (PIN * 2)); \
    PORT->OSPEEDR |= (SPEED << (PIN * 2)); \
    PORT->PUPDR &= ~(PUPDR << (PIN * 2)); \
    PORT->PUPDR |= (PUPDR << (PIN * 2)); \
    PORT->AFR[PIN / 8] &= ~(0x0F << ((PIN % 8) * 4)); \
	PORT->AFR[PIN / 8] |= (ALTER << ((PIN % 8) * 4)); \
}
*/

inline static void gpio_config(
		GPIO_TypeDef* port,
		uint32_t pin,
		GPIO_MODER_e moder,
		GPIO_OTYPER_e otyper,
		uint32_t speed,
		GPIO_PUPDR_e pupdr,
		uint32_t alter)
{
	static uint32_t temp = 0;
	temp = (moder << (pin * 2));
	port->MODER &= ~(moder << (pin * 2));
	port->MODER |= (moder << (pin * 2));

	temp = (port->MODER & ~(moder << (pin * 2))) | (moder << (pin * 2));
	//port->MODER = (port->MODER & ~(moder << (pin * 2))) | (moder << (pin * 2));
	port->OTYPER = (port->OTYPER & ~(otyper << pin)) | (otyper << pin);
	port->OSPEEDR = (port->OSPEEDR & ~(speed << (pin * 2))) | (speed << (pin * 2));
	port->PUPDR = (port->PUPDR & ~(pupdr << (pin * 2))) | (pupdr << (pin * 2));
	port->AFR[pin / 8] = (port->AFR[pin / 8] & ~(0x0F << (pin * 4))) | (alter << ((pin % 8) * 4));
}

#endif /* PERIPHERALS_GPIO_H_ */
