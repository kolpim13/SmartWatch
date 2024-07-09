#include "GUI_SettingsScreen.h"
#include "../nvm/nvm.h"
#include <FreeRTOS.h>
#include <queue.h>

extern RTC_HandleTypeDef hrtc;
extern QueueHandle_t EepromQueueHandle;
/*=================================================================*/

/* Pages content */
static void Menu_SettingsPageContent_TimeAndDate(lv_obj_t* content);

/* Callbacks */
static void event_TimeAndDate_DdTimeValueChanged(lv_event_t* event);
static void event_TimeAndDate_BtnApply_cb(lv_event_t* event);
/*=================================================================*/

/* MENU */
static lv_obj_t* menu = NULL;
static lv_obj_t* page_main = NULL;
static lv_obj_t* page_time = NULL;

/* Dropdown settings for time selection */
static const uint8_t* hours = "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23";
static const uint8_t* minutes_and_seconds = "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59\n";

static lv_obj_t* dd_hours;
static lv_obj_t* dd_minutes;
static lv_obj_t* dd_seconds;
static bool time_was_changed_bo = false;
/*=================================================================*/

static void Menu_SettingsPageContent_TimeAndDate(lv_obj_t* content)
{
    lv_obj_t* label;
    lv_obj_t* btn;

    /* Holder for any other elements on the page. */
    lv_obj_t* panel = lv_obj_create(content);
    lv_obj_set_size(panel, lv_display_get_horizontal_resolution(NULL), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);

    /* Add date selection. */
    label = lv_label_create(panel);
    lv_label_set_text(label, "Date");

    lv_obj_t* calendar = lv_calendar_create(panel);
    lv_obj_set_width(calendar, lv_display_get_horizontal_resolution(NULL) - 20);
    lv_obj_set_height(calendar, lv_display_get_horizontal_resolution(NULL) - 20);
    lv_calendar_header_dropdown_create(calendar);

    /* Add time selection */
    label = lv_label_create(panel);
    lv_label_set_text(label, "Hours");
    dd_hours = lv_dropdown_create(panel);
    lv_dropdown_set_options(dd_hours, hours);

    label = lv_label_create(panel);
    lv_label_set_text(label, "Minutes");
    dd_minutes = lv_dropdown_create(panel);
    lv_dropdown_set_options(dd_minutes, minutes_and_seconds);

    label = lv_label_create(panel);
    lv_label_set_text(label, "Seconds");
    dd_seconds = lv_dropdown_create(panel);
    lv_dropdown_set_options(dd_seconds, minutes_and_seconds);

    lv_obj_add_event_cb(dd_minutes, event_TimeAndDate_DdTimeValueChanged, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(dd_seconds, event_TimeAndDate_DdTimeValueChanged, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(dd_hours, event_TimeAndDate_DdTimeValueChanged, LV_EVENT_VALUE_CHANGED, NULL);

    /* Apply button */
    btn = lv_button_create(panel);
    label = lv_label_create(btn);
    lv_obj_center(label);
    lv_label_set_text(label, "Apply");
    lv_obj_add_event_cb(btn, event_TimeAndDate_BtnApply_cb, LV_EVENT_CLICKED, (void*)true);

    /* Go back to menu */
    btn = lv_button_create(panel);
    label = lv_label_create(btn);
    lv_obj_center(label);
    lv_label_set_text(label, "Back");
    lv_menu_set_load_page_event(menu, btn, page_main);
}

static void event_TimeAndDate_DdTimeValueChanged(lv_event_t* event)
{
    time_was_changed_bo = true;
}

static void event_TimeAndDate_BtnApply_cb(lv_event_t* event)
{
    /* If nothing was changed --> do not recalculate. */
    if (time_was_changed_bo == false)
    {
        return;
    }

    NvM_config_t* nvm = NvM_GetConfig();
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);

    /* Get data from drop down lists. */
    uint8_t buf[3];
    lv_dropdown_get_selected_str(dd_hours, buf, sizeof(buf));
    uint8_t hours = atoi(buf);
    lv_dropdown_get_selected_str(dd_minutes, buf, sizeof(buf));
    uint8_t minutes = atoi(buf);
    lv_dropdown_get_selected_str(dd_seconds, buf, sizeof(buf));
    uint8_t seconds = atoi(buf);

    /* Save it in eeprom */
    nvm->data.interpret.time.Hours = hours;
    nvm->data.interpret.time.Minutes = minutes;
    nvm->data.interpret.time.Seconds = seconds;
    NvM_Block_t block_type = NvM_Block_Time;
    if (xQueueSend (EepromQueueHandle,
                       (void *)&block_type,
                       (TickType_t)0) != pdPASS)
    {
        /* Add some notification for the user */
        // ...
        return;
    }

    /* Set time in RTC */
    sTime.Hours = hours;
    sTime.Minutes = minutes;
    sTime.Seconds = seconds;
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);

    time_was_changed_bo = false;
}
/*=================================================================*/

lv_obj_t* GUI_SettingsScreen_Create(void)
{
    /* Screen */
    lv_obj_t* screen = lv_obj_create(NULL);

    lv_obj_t* content;
    lv_obj_t* label;

    /* MENU */
    menu = lv_menu_create(screen);
    lv_menu_set_mode_header(menu, LV_MENU_HEADER_TOP_FIXED);
    lv_menu_set_mode_root_back_button(menu, LV_MENU_ROOT_BACK_BUTTON_DISABLED);
    lv_obj_set_size(lv_menu_get_main_header(menu), 0, 0);
    lv_obj_set_size(lv_menu_get_main_header_back_button(menu), 0, 0);
    lv_obj_set_width(menu, lv_display_get_horizontal_resolution(NULL));
    lv_obj_set_height(menu, lv_display_get_vertical_resolution(NULL));
    //lv_obj_align(menu, LV_ALIGN_TOP_MID, 0, 10) ;
    lv_obj_center(menu);

    /* ALL PAGES */
    page_main = lv_menu_page_create(menu, "Settings");
    page_time = lv_menu_page_create(menu, "Date and Time");

    /* TIME AND DATE PAGE */
    content = lv_menu_cont_create(page_time);
    Menu_SettingsPageContent_TimeAndDate(content);

    /* MAIN PAGE */
    content = lv_menu_cont_create(page_main);
    label = lv_label_create(content);
    lv_label_set_text(label, "Date and Time");
    lv_menu_set_load_page_event(menu, content, page_time);

    content = lv_menu_cont_create(page_main);
    label = lv_label_create(content);
    lv_label_set_text(label, "Dummy");

    /* LINKAGE BETWEEN PAGES */
    lv_menu_set_page(menu, page_main);

    return screen;
}
/*=================================================================*/
