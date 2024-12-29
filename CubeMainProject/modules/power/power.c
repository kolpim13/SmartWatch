#include "power.h"

#include "ST7789.h"
#include "../../thirdparty/lvgl/lv_port_disp.h"
#include "../../thirdparty/lvgl/lv_port_indev.h"
/*=================================================================*/

typedef enum
{
    Stage_LowPower_DisableUpdateLVGL = 0,
    Stage_LowPower_WaitLcdTransferFinish,
    Stage_LowPower_PauseTaskLVGl,
    Stage_LowPower_LcdToSleep,
} Stage_LowPower_t;

typedef enum 
{
    Stage_NormalMode_WakeUpLcd = 0,
    Stage_NormalMode_ResumeTaskLVGL,
    Stage_NormalMode_EnableUpdateLVGL,
    Stage_NormalMode_ResetCounter,
} Stage_NormalMode_t;
/*=================================================================*/

static volatile PWR_PowerMode_t power_mode = PWR_PowerMode_Normal;
static volatile bool enter_normal_mode_notify = false;
static volatile uint32_t low_power_limit_ms = 5000;
static volatile uint32_t low_power_counter_ms = 0;
/*=================================================================*/

static PWR_State_t enter_low_power_mode(void);
static PWR_State_t enter_normal_mode(void);
/*=================================================================*/

static PWR_State_t enter_low_power_mode(void)
{
    static Stage_LowPower_t state = Stage_LowPower_DisableUpdateLVGL;

    DEBUG_PIN_0_SET();

    switch (state)
    {
        case Stage_LowPower_DisableUpdateLVGL:
        {
            disp_disable_update();
            state = Stage_LowPower_WaitLcdTransferFinish;
        }
        case Stage_LowPower_WaitLcdTransferFinish:
        {
            /* Wait for all data transfer to be finished. */
            if (ST7789_CheckTrasferFinished() == false)
            {
                return PWR_State_Busy;
            }
            state = Stage_LowPower_PauseTaskLVGl;
        }
        case Stage_LowPower_PauseTaskLVGl:
        {
            vTaskSuspend(LvglTaskHandle);
            state = Stage_LowPower_LcdToSleep;
        }
        case Stage_LowPower_LcdToSleep:
        {
            ST7789_SleepIn();
            state = Stage_LowPower_DisableUpdateLVGL;

            break;
        }
    }

    DEBUG_PIN_0_RESET();
    
    return PWR_State_OK;
}

static PWR_State_t enter_normal_mode(void)
{
    static Stage_NormalMode_t state = Stage_NormalMode_WakeUpLcd;

    DEBUG_PIN_1_SET();

    switch(state)
    {
        case Stage_NormalMode_WakeUpLcd:
        {
            ST7789_SleepOut();
            state = Stage_NormalMode_EnableUpdateLVGL;
        }
        case Stage_NormalMode_EnableUpdateLVGL:
		{
			disp_enable_update();
			state = Stage_NormalMode_ResumeTaskLVGL;
		}
        case Stage_NormalMode_ResumeTaskLVGL:
        {
        	vTaskResume(LvglTaskHandle);
            state = Stage_NormalMode_ResetCounter;
        }
        case Stage_NormalMode_ResetCounter:
        {
            low_power_counter_ms = 0;
            state = Stage_NormalMode_WakeUpLcd;
            break;
        }
    }

    DEBUG_PIN_1_RESET();

    return PWR_State_OK;
}
/*=================================================================*/

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

void PWR_PowerMode_IncreaseCounter(uint32_t ms)
{
    low_power_counter_ms += ms;
}

void PWR_PowerMode_ResetCounter(void)
{
    low_power_counter_ms = 0;
}

void PWR_PowerMode_SetCounterLimit(uint32_t ms)
{

}

void PWR_PowerMode_EnterLowPowerMode_Notify(void)
{
    return;
}

void PWR_PowerMode_EnterNormalMode_Notify(void)
{
    if (power_mode == PWR_PowerMode_LowPower)
    {
        enter_normal_mode_notify = true;
    }
}

void PWR_Cyclic(void)
{
    static PWR_State_t state = PWR_State_OK;

    switch(power_mode)
    {
        case PWR_PowerMode_Normal:
        {
            DEBUG_PIN_0_SET();
            if (low_power_counter_ms >= low_power_limit_ms)
            {
                power_mode = PWR_PowerMode_ToLowPower;
            }
            DEBUG_PIN_0_RESET();
            break;
        }
        case PWR_PowerMode_LowPower:
        {
            DEBUG_PIN_1_SET();
            if (enter_normal_mode_notify == true)
            {
                power_mode = PWR_PowerMode_ToNormal;
                enter_normal_mode_notify = false;
            }
            DEBUG_PIN_1_RESET();
            break;
        }
        case PWR_PowerMode_ToLowPower:
        {
            DEBUG_PIN_2_SET();
            if (enter_low_power_mode() == PWR_State_OK)
            {
                power_mode = PWR_PowerMode_LowPower;
            }
            DEBUG_PIN_2_RESET();
            break;
        }
        case PWR_PowerMode_ToNormal:
        {
            DEBUG_PIN_3_SET();
            if (enter_normal_mode() == PWR_State_OK)
            {
                power_mode = PWR_PowerMode_Normal;
            }
            DEBUG_PIN_3_RESET();
            break;
        }
    }
}
/*=================================================================*/
