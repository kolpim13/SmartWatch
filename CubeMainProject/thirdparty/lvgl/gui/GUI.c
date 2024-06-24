#include "GUI.h"

void GUI_Init(void)
{
    /*Change the active screen's background color*/
    lv_obj_set_style_bg_color(lv_screen_active(), lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);

    /*Create a white label, set its text and align it to the center*/
    // lv_obj_t * label = lv_label_create(lv_screen_active());
    // lv_label_set_text(label, "Hello world");
    // lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffff), LV_PART_MAIN);
    // lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

}
