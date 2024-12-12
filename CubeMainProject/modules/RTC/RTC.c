#include "RTC.h"
#include "nvm.h"
/*=================================================================*/

static volatile bool notify_date_time_set = false;
static volatile bool notify_date_time_update = false;
static volatile bool notify_time_format_changed = false;
static volatile RTC_TimeFormat_t time_format;
static volatile RTC_TimeTypeDef sTime = {0};
static volatile RTC_DateTypeDef sDate = {0};
/*=================================================================*/

static void convert_time_to_12h_format(void);
/*=================================================================*/

void convert_time_to_12h_format(void)
{
    if (sTime.Hours > 12)
    {
        sTime.Hours -= 12;
        sTime.TimeFormat = RTC_HOURFORMAT12_PM;
    }
    else
    {
        sTime.TimeFormat = RTC_HOURFORMAT12_AM;
    }
}
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

    /* Read data from Eeprom and set them correspondingly */
    // Add check if eeprom data is valid and ready to be read
    // ...
    sTime.Hours = nvm_ram.data.rtc.Time.Hours;
    sTime.Minutes = nvm_ram.data.rtc.Time.Minutes;
    sTime.Seconds = nvm_ram.data.rtc.Time.Seconds;
    sTime.TimeFormat = (uint8_t)nvm_ram.data.rtc.time_format;

    /* Read from NvM error --> Write default data for time [TBD] */
    // ...

    /* Setup default value */
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;

    sDate.WeekDay = RTC_WEEKDAY_MONDAY;
    sDate.Month = RTC_MONTH_JANUARY;
    sDate.Date = 1;
    sDate.Year = 0;

    /* SETUP RTC values */
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_TIME_FORMAT);
    HAL_RTC_SetDate(&hrtc, &sDate, RTC_TIME_FORMAT);
}

void RTC_Cyclic_1s(void)
{
    static RTC_TimeTypeDef sTime_new = {0};
    static RTC_DateTypeDef sDate_new = {0};

    /* If Data or time was set */
    if (notify_date_time_set == true)
    {
        notify_date_time_set = false;
        HAL_RTC_SetDate(&hrtc, &sDate, RTC_TIME_FORMAT);
        HAL_RTC_SetTime(&hrtc, &sTime, RTC_TIME_FORMAT);
    }
    
    /* Get current time and date */
    if (notify_date_time_update == true)
    {
    	notify_date_time_update = false;
        HAL_RTC_GetTime(&hrtc, &sTime, RTC_TIME_FORMAT);
        HAL_RTC_GetDate(&hrtc, &sDate, RTC_TIME_FORMAT);

        /* Change Value according to current time format */
        if (time_format == RTC_HOURFORMAT_12)
        {
            convert_time_to_12h_format();
        }
    }
}

void RTC_DateTimeUpdate_Notify(void)
{
    notify_date_time_update = true;
}

void RTC_SetTimeFormat(RTC_TimeFormat_t format)
{
    time_format = format;
    notify_date_time_update = true;
}

RTC_TimeFormat_t RTC_GetTimeFormat(void)
{
    return time_format;
}

void RTC_SetTime(const RTC_TimeTypeDef* const time)
{
    sTime = *time;
    notify_date_time_set = true;
}

void RTC_SetDate(const RTC_DateTypeDef* const date)
{
    sDate = *date;
    notify_date_time_set = true;
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
