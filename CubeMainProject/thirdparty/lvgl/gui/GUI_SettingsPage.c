#include "GUI_SettingsPage.h"

#include "../modules/nvm/nvm.h"
#include "../modules/RTC/RTC.h"
#include "../modules/ST7789/ST7789.h"

typedef enum{
    User_Data_Display_Brightness = 0,
} User_Data_t;
/*=================================================================*/

static lv_obj_t* settings_page = NULL;
static lv_obj_t* menu;
static lv_obj_t* menu_main_page;
static lv_obj_t* menu_display_page;
static lv_obj_t* menu_datetime_page;

static uint8_t light_prev;
static RTC_TimeFormat_t time_format;
/*=================================================================*/

/* MAIN */
static void menu_page_main_create(void);

/* DISPLAY */
static void menu_page_display_create(void);
static void event_display_slider_brightness_cb(lv_event_t* event);
static void event_display_btn_apply_cb(lv_event_t* event);
static void event_display_btn_cancel_cb(lv_event_t* event);

/* DATE & TIME */
static void menu_page_datetime_create(void);
static void event_datetime_switch_timeformat_cb(lv_event_t* event);
static void event_datetime_btn_apply_cb(lv_event_t* event);
static void event_datetime_btn_cancel_cb(lv_event_t* event);

/* AUXILIARY FUNCTIONS */
// ...

/*=================================================================*/

/* MAIN */
static void menu_page_main_create()
{
    lv_obj_t* content;
    lv_obj_t* label;

    menu_main_page = lv_menu_page_create(menu, NULL);
    
    content = lv_menu_cont_create(menu_main_page);
    label = lv_label_create(content);
    lv_label_set_text(label, "Display");
    lv_menu_set_load_page_event(menu, content, menu_display_page);

    content = lv_menu_cont_create(menu_main_page);
    label = lv_label_create(content);
    lv_label_set_text(label, "Time & Date");
    lv_menu_set_load_page_event(menu, content, menu_datetime_page);

    content = lv_menu_cont_create(menu_main_page);
    label = lv_label_create(content);
    lv_label_set_text(label, "System [TBD]");   /* Energy saving settings. */

    content = lv_menu_cont_create(menu_main_page);
    label = lv_label_create(content);
    lv_label_set_text(label, "Info [TBD]");     /* Info about the project | SW & HW versions | CPU ID, etc. */
}

/* DISPLAY */
static void menu_page_display_create(void)
{
    lv_obj_t* content;
    lv_obj_t* slider;
    lv_obj_t* label;
    lv_obj_t* btn;

    /* Menu page */
    menu_display_page = lv_menu_page_create(menu, NULL);
    content = lv_menu_cont_create(menu_display_page);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(content, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    /* Brightness */
    light_prev = ST7789_GetLight();

    lv_obj_t *cont_brightness = lv_obj_create(content);
    lv_obj_set_size(cont_brightness, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(cont_brightness, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont_brightness, LV_FLEX_ALIGN_CENTER,  LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

    label = lv_label_create(cont_brightness);
    lv_label_set_text(label, "Brightness : ");

    slider = lv_slider_create(cont_brightness);
    lv_obj_set_width(slider, lv_pct(100));
    lv_slider_set_value(slider, light_prev, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider, event_display_slider_brightness_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* New setting [If some appear] */
    // ...

    /* Footer */
    btn = lv_msgbox_add_footer_button(content, "Apply");
    lv_obj_set_flex_grow(btn, 1);
    lv_obj_add_event_cb(btn, event_display_btn_apply_cb, LV_EVENT_CLICKED, (void *)slider);

    btn = lv_msgbox_add_footer_button(content, "Cancel");
    lv_obj_set_flex_grow(btn, 1);
    lv_obj_add_event_cb(btn, event_display_btn_cancel_cb, LV_EVENT_CLICKED, (void *)slider);
}

static void event_display_slider_brightness_cb(lv_event_t* event)
{
    lv_obj_t *slider = lv_event_get_target(event);
    uint8_t light = lv_bar_get_value(slider);
    ST7789_SetLight(light);
}

static void event_display_btn_apply_cb(lv_event_t* event)
{
    lv_obj_t *slider = lv_event_get_user_data(event);
    uint8_t light = lv_bar_get_value(slider);

    /* If there were differences --> save them. */
    if (light_prev != light)
    {

        /* Send data to be saved */
        NvM_Block_t nvm_block = NvM_Block_Display;
        nvm_ram.data.display.brightness_pc = light;
        if (xQueueSend(
                EepromQueueHandle,
                (void*)&nvm_block,
                (TickType_t)0) != pdPASS)
        {
            /* Add some notification for the user */
            // ...
            ;
        }

        /* Write current level of the brightness. */
        light_prev = light;
    }

    lv_menu_set_page(menu, menu_main_page);
}

static void event_display_btn_cancel_cb(lv_event_t* event)
{
    lv_obj_t *slider = lv_event_get_user_data(event);
    ST7789_SetLight(light_prev);
    lv_slider_set_value(slider, light_prev, LV_ANIM_OFF);
    lv_menu_set_page(menu, menu_main_page);
}

/* DATE & TIME */
static void menu_page_datetime_create(void)
{
    lv_obj_t* content;
    lv_obj_t* sw;
    lv_obj_t* label;
    lv_obj_t* btn;

    /* Menu page */
    menu_datetime_page = lv_menu_page_create(menu, NULL);
    content = lv_menu_cont_create(menu_datetime_page);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(content, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    /* Time format */
    time_format = RTC_GetTimeFormat();

    lv_obj_t *cont_timeformat = lv_obj_create(content);
    lv_obj_set_size(cont_timeformat, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(cont_timeformat, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_timeformat, LV_FLEX_ALIGN_START,  LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_remove_flag(cont_timeformat, LV_OBJ_FLAG_SCROLLABLE);

    label = lv_label_create(cont_timeformat);
    lv_label_set_text(label, "Time foramt:");

    sw = lv_switch_create(cont_timeformat);
    label = lv_label_create(cont_timeformat);
    if (time_format == RTC_TimeFormat_24H)
    {
        lv_label_set_text_static(label, "24H ");
        lv_obj_remove_state(sw, LV_STATE_CHECKED);
    }
    else
    {
        lv_label_set_text_static(label, "12H ");
        lv_obj_add_state(sw, LV_STATE_CHECKED);
    }
    lv_obj_add_event_cb(sw, event_datetime_switch_timeformat_cb, LV_EVENT_VALUE_CHANGED, (void *)label);

    /* Footer */
    btn = lv_msgbox_add_footer_button(content, "Apply");
    lv_obj_set_flex_grow(btn, 1);
    lv_obj_add_event_cb(btn, event_datetime_btn_apply_cb, LV_EVENT_CLICKED, NULL);

    btn = lv_msgbox_add_footer_button(content, "Cancel");
    lv_obj_set_flex_grow(btn, 1);
    lv_obj_add_event_cb(btn, event_datetime_btn_cancel_cb, LV_EVENT_CLICKED, NULL);
}

static void event_datetime_switch_timeformat_cb(lv_event_t* event)
{
    lv_obj_t* sw = lv_event_get_target(event);
    lv_obj_t* label = lv_event_get_user_data(event);

    if (lv_obj_has_state(sw, LV_STATE_CHECKED) == true)
    {
        lv_label_set_text_static(label, "12H ");
        time_format = RTC_HOURFORMAT_12;
    }
    else
    {
        lv_label_set_text_static(label, "24H ");
        time_format = RTC_HOURFORMAT_24;
    }
}

static void event_datetime_btn_apply_cb(lv_event_t* event)
{
    /* Change data format now */
    RTC_SetTimeFormat(time_format);

    /* Update status bar with new time format. */
    GUI_StatusBar_Update();

    /* Write to the NvM. */
    NvM_Block_t nvm_block = NvM_Block_RTC;
    nvm_ram.data.rtc.time_format = time_format;
    if (xQueueSend(
            EepromQueueHandle,
            (void*)&nvm_block,
            (TickType_t)0) != pdPASS)
    {
        /* Add some notification for the user */
        // ...
        ;
    }

    lv_menu_set_page(menu, menu_main_page);
}

static void event_datetime_btn_cancel_cb(lv_event_t* event)
{
    lv_menu_set_page(menu, menu_main_page);
}

/* AUXILIARY FUNCTIONS */
// ...

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

    /* Menu */
    menu = lv_menu_create(settings_page);
    lv_menu_set_mode_header(menu, LV_MENU_HEADER_TOP_FIXED);
    lv_menu_set_mode_root_back_button(menu, LV_MENU_ROOT_BACK_BUTTON_DISABLED);
    lv_obj_set_size(lv_menu_get_main_header(menu), 0, 0);
    lv_obj_set_size(lv_menu_get_main_header_back_button(menu), 0, 0);
    lv_obj_set_size(menu, lv_obj_get_width(lv_scr_act()), lv_obj_get_height(lv_scr_act() - 30));
    lv_obj_align(menu, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_set_style_border_opa(menu, LV_OPA_TRANSP, 0);
    lv_obj_set_scrollbar_mode(menu, LV_SCROLLBAR_MODE_OFF);

    /* Menu pages */
    menu_page_display_create();
    menu_page_datetime_create();
    menu_page_main_create();

    /* Set main page as default */
    lv_menu_set_page(menu, menu_main_page);
}
