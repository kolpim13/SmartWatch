#include "GUI_MainPage.h"
#include "../modules/RTC/RTC.h"

extern const lv_font_t lv_font_debrosee_digitsonly_40;

static lv_obj_t* main_page = NULL;
static lv_obj_t* time_label = NULL;
static lv_obj_t* date_label = NULL;
static lv_obj_t* update_timer = NULL;
/*=================================================================*/

/**
 * @brief 
 * 
 * @return true - was updated
 * @return false - was not updated
 */
static bool update_time_label(void);

/**
 * @brief 
 * 
 * @return true - was updated
 * @return false - was not updated
 */
static bool update_date_label(void);

/**
 * @brief Update an entire page.
 * Non-reentrant
 * Should be called periodically
 */
static void update_page_1s(void);
/*=================================================================*/

static bool update_time_label(void)
{
    static RTC_TimeTypeDef time_prev = {0};

    /* Get current date and time*/
    const RTC_TimeTypeDef* time = RTC_GetTime();

    /* If seconds differ --> Update time label */
    if (time->Seconds != time_prev.Seconds)
    {
        time_prev = *time;
        lv_label_set_text_fmt(
            time_label, "%02u:%02u:%02u",
            RTC_BCDtoByte(time->Hours), 
            RTC_BCDtoByte(time->Minutes),
            RTC_BCDtoByte(time->Seconds)
        );
        return true;
    }
    return false;
}

static bool update_date_label(void)
{
    static RTC_DateTypeDef date_prev = {0};
    static const char* day_names[] = LV_CALENDAR_DEFAULT_DAY_NAMES;

    const RTC_DateTypeDef* date = RTC_GetDate();

    /* If date changed --> update label. */
    if (date->Date != date_prev.Date)
    {
        lv_label_set_text_fmt(
            date_label, "%u\\%u %s",
            date->Date, date->Month, day_names[date->WeekDay]
        );
        return true;
    }
    return false;
}

static void update_page_1s(void)
{
    /* If time was not changed --> date remain also the same. */
    if (update_time_label() == false)
    {
        return;
    }
    update_date_label();
}
/*=================================================================*/

void GUI_MainPage_Create(lv_obj_t* parent)
{
    if (main_page != NULL)
    {
        return;
    }

    /* Page itself */
    main_page = lv_obj_create(parent);
    lv_obj_set_size(main_page, lv_obj_get_width(lv_scr_act()), lv_obj_get_height(lv_scr_act()));
    lv_obj_center(main_page);
    lv_obj_set_scrollbar_mode(main_page, LV_SCROLLBAR_MODE_OFF);

    /* Time label */
    time_label = lv_label_create(main_page);
    lv_obj_set_width(time_label, lv_display_get_horizontal_resolution(NULL) - 10);
    lv_obj_set_style_text_align(time_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(time_label, &lv_font_debrosee_digitsonly_40, 0);
    lv_obj_align(time_label, LV_ALIGN_TOP_MID, 0, 40);
    lv_label_set_text(time_label, "");

    /* Date label */
    date_label = lv_label_create(main_page);
    lv_obj_set_width(date_label, lv_display_get_horizontal_resolution(NULL) - 10);
    lv_obj_set_style_text_align(date_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(date_label, &lv_font_montserrat_24, 0);
    lv_obj_align_to(date_label, time_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_label_set_text(date_label, "");

    /* Update Timer */
    update_timer = lv_timer_create_basic();
    lv_timer_set_cb(update_timer, update_page_1s);
    lv_timer_set_period(update_timer, 500);
    lv_timer_ready(update_timer);
}
/*=================================================================*/
