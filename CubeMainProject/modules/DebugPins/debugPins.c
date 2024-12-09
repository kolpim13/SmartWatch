#include "debugPins.h"

void DebugPins_GPIO_Init(void)
{
    #if HW_VERSION == HW_1V0 || HW_VERSION == HW_1V1

    /* Configure all pins on port A. */
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    GPIO_InitStructure.Pin = (DEBUG_PIN_0 | DEBUG_PIN_1);
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure all pins on port B. */
    GPIO_InitStructure.Pin = (DEBUG_PIN_2 | DEBUG_PIN_3 | DEBUG_PIN_4);
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    #endif
}
/*=================================================================*/
