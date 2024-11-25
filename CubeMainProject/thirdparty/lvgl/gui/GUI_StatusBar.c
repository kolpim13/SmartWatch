#include "GUI_StatusBar.h"

void GUI_StatusBar_Create(lv_obj_t* parent)
{
    /* Create the status bar container.
    Fixed at the top of the screen, 30 px height */
    status_bar = lv_obj_create(parent);
    lv_obj_set_size(status_bar, LV_HOR_RES, 30);      
    lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 0);

    /* Widget for current time. */
    lv_obj_t* time_label = lv_label_create(status_bar);
    

    /* Widget for battery status.
    Implemented as a bar. */
    lv_obj_t* battery_bar = lv_bar_create(status_bar);
    lv_bar_set_range(battery_bar, 0, 100);                  // Battery range (0 to 100%)
    lv_bar_set_value(battery_bar, 100, LV_ANIM_OFF);        // Initial value (100%)
    lv_obj_align(battery_bar, LV_ALIGN_RIGHT_MID, 10, 0);
    lv_obj_set_size(battery_bar, 100, 20);                  // (?) Width x Height of the bar
}
