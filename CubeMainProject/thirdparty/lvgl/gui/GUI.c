#include "GUI.h"

static lv_obj_t* screen = NULL;
/*=================================================================*/

void GUI_Init(void)
{
    screen = lv_obj_create(NULL);

	/* Create tileview that is main holder for screen layout. */
	lv_obj_t* tileview = lv_tileview_create(screen);
	lv_obj_set_scrollbar_mode(tileview, LV_SCROLLBAR_MODE_OFF);

	// lv_obj_set_size(tileview, lv_obj_get_width(lv_scr_act()), lv_obj_get_height(lv_scr_act()));

	/* Add tiles (can be considered as pages / screens). */
	lv_obj_t* tile_main = lv_tileview_add_tile(tileview, 0, 0, LV_DIR_RIGHT);
    lv_obj_t* tile_settings = lv_tileview_add_tile(tileview, 1, 0, LV_DIR_LEFT);

	/* Fill tiles with content. */
	GUI_MainPage_Create(tile_main);
	GUI_SettingsPage_Create(tile_settings);
	// ... Another pages

	/* Set "main" page as a default view. */
	lv_tileview_set_tile(tileview, tile_main, LV_ANIM_OFF);

	/* Load screen we operating on */
    lv_screen_load(screen);

	/* Create status bar on top */
	GUI_StatusBar_Create();
}
/*=================================================================*/

/* LVGL TIMERS */
void timer_statusBar_update(lv_timer_t* timer)
{
	(void)timer;
	GUI_StatusBar_Update_Cyclic_1s();
}
/*=================================================================*/
