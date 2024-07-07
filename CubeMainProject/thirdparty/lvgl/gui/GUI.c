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
    screen_main = GUI_MainScreen_Create();
    screen_settings = GUI_SettingsScreen_Create();
    lv_screen_load(screen_main);

    /* Navigation between screens. */
    lv_obj_add_event_cb(screen_main, event_screen_navigation_gesture, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(screen_settings, event_screen_navigation_gesture, LV_EVENT_GESTURE, NULL);
}
