#include "GUI_StatusBar.h"
#include "../modules/RTC/RTC.h"
#include "../modules/power/power.h"

#define MAKE_COLOR_GRAY_LIGHT()     lv_color_make(200, 200, 200)
#define MAKE_COLOR_GREEN()          lv_color_make(0, 255, 0)
#define MAKE_COLOR_GREEN_LIGHT()    lv_color_make(144, 238, 144)
/*=================================================================*/

static lv_obj_t* status_bar = NULL;
static lv_obj_t* time_label;
static lv_obj_t* battery_bar;
static lv_obj_t* battery_label;
static lv_obj_t* update_timer = NULL;

static volatile bool notify_time_upd = false;
/*=================================================================*/

/* INIT */
/**
 * @brief 
 * 
 * @return lv_obj_t* 
 */
static lv_obj_t* battery_bar_create(void);

/* EVENTS and CYCLICAL */
/**
 * @brief Update all info on status bar.
 * Non-reentrant
 * Should be called periodically.
 */
static void update_status_bar(void);

/**
 * @brief Generic callback for status bar widget.
 * 
 * @param event 
 */
static void event_generic_status_bar_cb(lv_event_t* event);
/*=================================================================*/

static lv_obj_t* battery_bar_create()
{
    static lv_style_t battery_bar_style_border;
    static lv_style_t battery_bar_style_bar;

    /* Border style */
    lv_style_init(&battery_bar_style_border);
    lv_style_set_border_color(&battery_bar_style_border, lv_color_black());
    lv_style_set_border_width(&battery_bar_style_border, 2);
    lv_style_set_pad_all(&battery_bar_style_border, 3);     /*To make the indicator smaller*/
    lv_style_set_radius(&battery_bar_style_border, 8);
    lv_style_set_anim_duration(&battery_bar_style_border, 1000);

    /* Bar style */
    lv_style_init(&battery_bar_style_bar);
    lv_style_set_bg_opa(&battery_bar_style_bar, LV_OPA_COVER);
    lv_style_set_bg_color(&battery_bar_style_bar, MAKE_COLOR_GRAY_LIGHT());
    lv_style_set_radius(&battery_bar_style_bar, 8);

    /* Battery bar */
    battery_bar = lv_bar_create(status_bar);
    lv_obj_remove_style_all(battery_bar);  /*To have a clean start */
    lv_obj_add_style(battery_bar, &battery_bar_style_border, LV_PART_MAIN);
    lv_obj_add_style(battery_bar, &battery_bar_style_bar, LV_PART_INDICATOR);

    lv_bar_set_range(battery_bar, 0, 100);              
    lv_bar_set_value(battery_bar, 100, LV_ANIM_OFF);    
    lv_bar_set_mode(battery_bar, LV_BAR_MODE_NORMAL);   

    lv_obj_align(battery_bar, LV_ALIGN_RIGHT_MID, -15, 0);  /* physical display is rounded --> a little indent is needed. */
    lv_obj_set_size(battery_bar, 40, 20);

    /* Specific font should be added. [Small size and digits + ": ; . , #" only]*/
    battery_label = lv_label_create(battery_bar);
    lv_label_set_text(battery_label, "100");
    lv_obj_align(battery_label, LV_ALIGN_CENTER, 0, 0);  // Center the label inside the bar

    return battery_bar;
}

static void update_status_bar(void)
{
    static RTC_TimeTypeDef time_prev = {0};
    static bool charge_active_prev = false;

    /* Whole block without entering nested conditions 1.5 - 2 uS. */

    /* Get current date and time*/
    const RTC_TimeTypeDef* time = RTC_GetTime();

    /* If seconds differ --> Update time label */
    notify_time_upd = time->Minutes != time_prev.Minutes ? true : notify_time_upd;
    if (notify_time_upd == true)
    {
        /* 425 - 430 uS.
        Remeasure it after time formatting was added. */
        time_prev = *time;
        lv_label_set_text_fmt(
            time_label, "%02u:%02u %s",
            RTC_BCDtoByte(time->Hours), 
            RTC_BCDtoByte(time->Minutes),
            RTC_GetTimeFormat() == RTC_TimeFormat_24H ? "" : time->TimeFormat == RTC_HOURFORMAT12_AM ? "am" : "pm"
        );
    }

    /* Update battery bar.
    Percentage of the charge is not implemented yet (for bith HW and SW). */
    // ...
    
    /* Get state of the charge. */
    bool charge_active = PWR_IsChargeActive();
    if (charge_active != charge_active_prev)
    {
        /* 140 - 145 uS. */
        /* If charge connected --> make battery bar green */
        if (charge_active == true)
        {
            lv_obj_set_style_bg_color(battery_bar, MAKE_COLOR_GREEN_LIGHT(), LV_PART_INDICATOR);
        }
        /* If not --> Return light gray color. */
        else
        {
            lv_obj_set_style_bg_color(battery_bar, MAKE_COLOR_GRAY_LIGHT(), LV_PART_INDICATOR);
        }
        charge_active_prev = charge_active;
    }
}

static void event_generic_status_bar_cb(lv_event_t* event)
{
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_REFRESH)
    {
        update_status_bar();
    }
}
/*=================================================================*/

void GUI_StatusBar_Create(void)
{
    /* If one status bar was created --> do not create another one. */
    if (status_bar != NULL) { return; }

    /* Create the status bar container.
    Fixed at the top of the screen, 30 px height */
    status_bar = lv_obj_create(lv_layer_top());
    lv_obj_set_size(status_bar, LV_HOR_RES, 30);      
    lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_scrollbar_mode(status_bar, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(status_bar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(status_bar, LV_OPA_TRANSP, 0);      // Make the background fully transparent
    lv_obj_set_style_border_opa(status_bar, LV_OPA_TRANSP, 0);  // Make the border fully transparent

    /* TIME [Label]. 
    Aligned to very left of the status bar. */
    time_label = lv_label_create(status_bar);
    lv_label_set_text(time_label, "00:00");                     // Initial time
    lv_obj_align(time_label, LV_ALIGN_LEFT_MID, 10, 0);   // Align to very left

    /* BATTERY STATUS [Bar].
    [Right now there is no available correctly working battery ]*/
    lv_obj_t* battery_bar = battery_bar_create();

    /* Update timer */
    update_timer = lv_timer_create_basic();
    lv_timer_set_cb(update_timer, update_status_bar);
    lv_timer_set_period(update_timer, 1000);
    lv_timer_ready(update_timer);

    /* Add events for status bar */
    lv_obj_add_event_cb(status_bar, event_generic_status_bar_cb, LV_EVENT_REFRESH, NULL);
}

void GUI_StatusBar_Update(void)
{
	notify_time_upd = true;
    lv_obj_send_event(status_bar, LV_EVENT_REFRESH, NULL);
}
/*=================================================================*/
