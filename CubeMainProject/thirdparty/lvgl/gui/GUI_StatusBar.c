#include "GUI_StatusBar.h"
/*=================================================================*/


/*=================================================================*/

void GUI_StatusBar_Create(lv_obj_t* screen)
{
    /* Create the status bar container.
    Fixed at the top of the screen, 30 px height */
    status_bar = lv_obj_create(screen);
    lv_obj_set_size(status_bar, LV_HOR_RES, 30);      
    lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 0);

    /* Widget for current time. 
    Aligned to very left of the status bar. */
    lv_obj_t* time_label = lv_label_create(status_bar);
    lv_label_set_text(time_label, "00:00")                      // Initial time
    lv_obj_align(time_label, NULL, LV_ALIGN_LEFT_MID, 10, 0);   // Align to very left

    /* Widget for battery status.
    Implemented as a bar. */
    lv_obj_t* battery_bar = lv_bar_create(status_bar);
    lv_bar_set_range(battery_bar, 0, 100);                  
    lv_bar_set_value(battery_bar, 100, LV_ANIM_OFF);
    lv_bar_set_mode(battery_bar, LV_BAR_MODE_NORMAL);      
    lv_bar_set_orientation(battery_bar, LV_BAR_ORIENTATION_HORIZONTAL);

    lv_obj_align(battery_bar, LV_ALIGN_RIGHT_MID, -10, 0);
    lv_obj_set_size(battery_bar, 100, 20);                  // (?) Width x Height of the bar

    lv_obj_t* battery_label = lv_label_create(battery_bar);
    lv_label_set_text(battery_label, "100");
    lv_obj_align(battery_label, LV_ALIGN_CENTER, 0, 0);  // Center the label inside the bar
}

void GUI_StatusBar_Update_Cyclic_1s(void)
{
    static RTC_TimeTypeDef time_prev = {0};

    /* Get current date and time*/
    const RTC_TimeTypeDef* time = RTC_GetTime();

    /* If seconds differ --> Update time label */
    if (time->Seconds != time_prev.Seconds)
    {
        
    } 

}
/*=================================================================*/
