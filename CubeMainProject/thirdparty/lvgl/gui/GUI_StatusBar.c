#include "GUI_StatusBar.h"
#include "../modules/RTC/RTC.h"
/*=================================================================*/

static lv_obj_t* status_bar = NULL;
static lv_obj_t* time_label;
static lv_obj_t* battery_label;
static lv_obj_t* update_timer = NULL;
/*=================================================================*/

/**
 * @brief Update all info on status bar.
 * Non-reentrant
 * Should be called periodically.
 */
static void update_status_bar_1s(void);
/*=================================================================*/

static void update_status_bar_1s(void)
{
    static RTC_TimeTypeDef time_prev = {0};

    /* Get current date and time*/
    const RTC_TimeTypeDef* time = RTC_GetTime();

    /* If seconds differ --> Update time label */
    if (time->Minutes != time_prev.Minutes)
    {
        time_prev = *time;
        lv_label_set_text_fmt(
            time_label, "%02u:%02u",
            RTC_BCDtoByte(time->Hours), 
            RTC_BCDtoByte(time->Minutes)
        );
    }

    /* Update battery bar */
    // ... When power indication will be working nice.
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

    /* Widget for current time. 
    Aligned to very left of the status bar. */
    time_label = lv_label_create(status_bar);
    lv_label_set_text(time_label, "00:00");                     // Initial time
    lv_obj_align(time_label, LV_ALIGN_LEFT_MID, 10, 0);   // Align to very left

    /* Widget for battery status.
    Implemented as a bar. 
    [Right now there is no available correctly working battery ]*/
    lv_obj_t* battery_bar = lv_bar_create(status_bar);
    lv_bar_set_range(battery_bar, 0, 100);                  
    lv_bar_set_value(battery_bar, 100, LV_ANIM_OFF);
    lv_bar_set_mode(battery_bar, LV_BAR_MODE_NORMAL);

    lv_obj_align(battery_bar, LV_ALIGN_RIGHT_MID, -10, 0);
    lv_obj_set_size(battery_bar, 50, 20);                  // (?) Width x Height of the bar

    /* Specific font should be added. [Small size and digits + ": ; . , #" only]*/
    battery_label = lv_label_create(battery_bar);
    lv_label_set_text(battery_label, "100");
    lv_obj_align(battery_label, LV_ALIGN_CENTER, 0, 0);  // Center the label inside the bar

    /* Update timer */
    update_timer = lv_timer_create_basic();
    lv_timer_set_cb(update_timer, update_status_bar_1s);
    lv_timer_set_period(update_timer, 500);
    lv_timer_ready(update_timer);
}
/*=================================================================*/
