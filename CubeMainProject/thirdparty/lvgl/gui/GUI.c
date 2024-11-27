#include "GUI.h"

static lv_obj_t* screen_main = NULL;
static lv_obj_t* screen_settings = NULL;
/*=================================================================*/

static void event_screen_navigation_gesture(lv_event_t* event);
/*=================================================================*/

static void event_screen_navigation_gesture(lv_event_t* event)
{
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());

    lv_obj_t* screen = lv_event_get_current_target(event);
    if (screen == screen_main)
    {
	    switch (dir)
		{
			case LV_DIR_LEFT:
			{
				lv_screen_load_anim(screen_settings, LV_SCR_LOAD_ANIM_OVER_LEFT, 1000, 0, false);
				break;
			}
			default: { break; }
		}
    }
    else if (screen == screen_settings)
    {
        switch (dir)
		{
			case LV_DIR_RIGHT:
			{
				lv_screen_load_anim(screen_main, LV_SCR_LOAD_ANIM_OVER_RIGHT, 1000, 0, false);
				break;
			}
			default: { break; }
		}
    }

}
/*=================================================================*/

void GUI_Init(void)
{
    screen_main = GUI_MainScreen_Create(); //GUI_ScreenMain_Create();
    lv_screen_load(screen_main);
	GUI_StatusBar_Create();
}
/*=================================================================*/

/* LVGL TIMERS */
void timer_statusBar_update(lv_timer_t* timer)
{
	(void)timer;
	GUI_StatusBar_Update_Cyclic_1s();
}
