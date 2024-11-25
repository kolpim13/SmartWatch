#include "RTC.h"
#include "nvm.h"
/*=================================================================*/

static volatile bool notify_date_time_update = false;
static volatile RTC_TimeTypeDef sTime = {0};
static volatile RTC_DateTypeDef sDate = {0};
/*=================================================================*/

void RTC_Init(void)
{
    /* Initialize RTC */
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 127;
    hrtc.Init.SynchPrediv = 255;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    if (HAL_RTC_Init(&hrtc) != HAL_OK)
    {
        Error_Handler();
    }

    /* If RTC was configured ==> continue from the moment it stopped. */
    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_STATE_BKP_REGISTER) == RTC_STATE_ENABLE)
    {
        return;
    }

    /* Mark RTC as enabled. */
    HAL_PWR_EnableBkUpAccess();
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_STATE_BKP_REGISTER, RTC_STATE_ENABLE);
    HAL_PWR_DisableBkUpAccess();

    /* Setup default value */
    sTime.Hours = 0;
    sTime.Minutes = 0;
    sTime.Seconds = 0;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;

    sDate.WeekDay = RTC_WEEKDAY_MONDAY;
    sDate.Month = RTC_MONTH_JANUARY;
    sDate.Date = 1;
    sDate.Year = 0;

    /* Read data from Eeprom and set them correspondingly */
    // Add check if eeprom data is valid and ready to be read
    // ...
    sTime.Hours = nvm_ram.data.rtc.Time.Hours;
    sTime.Minutes = nvm_ram.data.rtc.Time.Minutes;
    sTime.Seconds = nvm_ram.data.rtc.Time.Seconds;

    /* SETUP RTC values */
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_TIME_FORMAT);
    HAL_RTC_SetDate(&hrtc, &sDate, RTC_TIME_FORMAT);
}

void RTC_Cyclic_1s(void)
{
    static RTC_TimeTypeDef sTime_new = {0};
    static RTC_DateTypeDef sDate_new = {0};

    /* Get current time and date */
    if (notify_date_time_update == true)
    {
        HAL_RTC_GetTime(&hrtc, &sTime, RTC_TIME_FORMAT);
        HAL_RTC_GetDate(&hrtc, &sDate, RTC_TIME_FORMAT);
    }
}

void RTC_DateTimeUpdate_Notify(void)
{
    notify_date_time_update = true;
}

void RTC_SetTime(const RTC_TimeTypeDef* const time)
{
    sTime = *time;
}

void RTC_SetDate(const RTC_DateTypeDef* const date)
{
    sDate = *date;
}

const RTC_TimeTypeDef* RTC_GetTime(void)
{
    return (const RTC_TimeTypeDef*)&sTime;
}

const RTC_DateTypeDef* RTC_GetDate(void)
{
    return (const RTC_DateTypeDef*)&sDate;
}
/*=================================================================*/
