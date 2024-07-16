#include "GUI_MainScreen.h"

// LV_FONT_DEBROSEE_DIGITSONLY_40_DECLARE()

extern const lv_font_t lv_font_debrosee_digitsonly_40;

static lv_obj_t* time_lb = NULL;
static lv_obj_t* date_lb = NULL;
/*=================================================================*/

/**
 * @brief
 *
 * @param event
 */
static void event_time_refresh_cb(lv_event_t* event);

/**
 * @brief 
 * 
 * @param event 
 */
static void event_date_refresh_cb(lv_event_t* event);
/*=================================================================*/

static void event_time_refresh_cb(lv_event_t* event)
{
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    /* Both GetTime and GetDate should be called simultaneousely to refresh time correctly. */
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);

    /* Casting is needed. Every 10 seconds HAL rounds results to hex format. So 10 seconds == 16. */
    lv_label_set_text_fmt(time_lb, "%02u:%02u:%02u",
    		(((sTime.Hours / 16) * 10) + (sTime.Hours % 16)),
			(((sTime.Minutes / 16) * 10) + (sTime.Minutes % 16)),
			(((sTime.Seconds / 16) * 10) + (sTime.Seconds % 16)));

    return;
}

static void event_date_refresh_cb(lv_event_t* event)
{
    static const char* day_names[] = LV_CALENDAR_DEFAULT_DAY_NAMES;

    RTC_DateTypeDef sDate = {0};
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);

    lv_label_set_text_fmt(date_lb, "%u\\%u %s",
    		sDate.Date, sDate.Month, day_names[sDate.WeekDay]);
}
/*=================================================================*/

lv_obj_t* GUI_MainScreen_Create(void)
{
    /* Screen */
    lv_obj_t* screen = lv_obj_create(NULL);

    /* Label with time. */
    time_lb = lv_label_create(screen);
    lv_obj_set_width(time_lb, lv_display_get_horizontal_resolution(NULL) - 10);
    lv_obj_set_style_text_align(time_lb, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(time_lb, &lv_font_debrosee_digitsonly_40, 0);
    lv_obj_align(time_lb, LV_ALIGN_TOP_MID, 0, 20);
    lv_label_set_text(time_lb, "");

    // /* Label with date. */
    date_lb = lv_label_create(screen);
    lv_obj_set_width(date_lb, lv_display_get_horizontal_resolution(NULL) - 10);
    lv_obj_set_style_text_align(date_lb, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(date_lb, &lv_font_montserrat_24, 0);
    lv_obj_align(date_lb, LV_ALIGN_TOP_MID, 0, 50);
    lv_label_set_text(date_lb, "");

    /* Add callbacks for time update. */
    lv_obj_add_event_cb(time_lb, event_time_refresh_cb, LV_EVENT_REFRESH, NULL);
    lv_obj_add_event_cb(date_lb, event_date_refresh_cb, LV_EVENT_REFRESH, NULL);

    return screen;
}

void GUI_MainScreen_UpdateDateAndTime_Notify(void)
{
    if (time_lb == NULL ||
        date_lb == NULL)
    {
        return;
    }

    lv_obj_send_event(time_lb, LV_EVENT_REFRESH, NULL);
    lv_obj_send_event(date_lb, LV_EVENT_REFRESH, NULL);
}
/*=================================================================*/
