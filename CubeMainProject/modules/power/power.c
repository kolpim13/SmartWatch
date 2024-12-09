#include "power.h"

void PWR_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = PWR_EN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(PWR_EN_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PWR_CHARGE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(PWR_CHARGE_PORT, &GPIO_InitStruct);
}

bool PWR_IsChargeActive(void)
{
    #if HW_VERSION < HW_1V1
    return false;
    #else
    return (bool)HAL_GPIO_ReadPin(PWR_CHARGE_PORT, PWR_CHARGE_PIN);
    #endif
}
/*=================================================================*/
