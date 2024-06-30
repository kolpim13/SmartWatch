#include "GUI_MainPage.h"

extern RTC_HandleTypeDef hrtc;

static lv_obj_t* time_lb = NULL;

static void event_time_lb_refresh_cb(lv_event_t* event);

static void event_time_lb_refresh_cb(lv_event_t* event)
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

void GUI_MainPage_Create(void)
{
    time_lb = lv_label_create(lv_screen_active());
    lv_obj_set_width(time_lb, 220);
    lv_obj_set_style_text_align(time_lb, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(time_lb, LV_ALIGN_TOP_MID, 0, 0);
    lv_label_set_long_mode(time_lb, LV_LABEL_LONG_DOT);
    lv_label_set_text(time_lb, "test");

    lv_obj_add_event_cb(time_lb, event_time_lb_refresh_cb, LV_EVENT_REFRESH, NULL);
}

void GUI_MainPage_UpdateTime(void)
{
    if (time_lb == NULL)
    {
        return;
    }

    lv_obj_send_event(time_lb, LV_EVENT_REFRESH, NULL);
}
