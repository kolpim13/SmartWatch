#include "GUI_SettingsPage.h"

static lv_obj_t* settings_page = NULL;
/*=================================================================*/

void GUI_SettingsPage_Create(lv_obj_t* parent)
{
    if (settings_page != NULL)
    {
        return;
    }

    /* Page itself */
    settings_page = lv_obj_create(parent);
    lv_obj_set_size(settings_page, lv_obj_get_width(lv_scr_act()), lv_obj_get_height(lv_scr_act()));
    lv_obj_center(settings_page);
    lv_obj_set_scrollbar_mode(settings_page, LV_SCROLLBAR_MODE_OFF);

    /*  */
    lv_obj_t* time_label = lv_label_create(settings_page);
    lv_obj_set_width(time_label, lv_display_get_horizontal_resolution(NULL) - 10);
    lv_obj_set_style_text_align(time_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_24, 0);
    lv_obj_align(time_label, LV_ALIGN_TOP_MID, 0, 40);
    lv_label_set_text(time_label, "Bla-bla-bla");
}