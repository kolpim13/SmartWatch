/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include "ST7789.h"
#include <stdbool.h>

/*********************
 *      DEFINES
 *********************/
#define MY_DISP_HOR_RES    ST7789_LCD_WIDTH
#define MY_DISP_VER_RES    ST7789_LCD_HEIGHT
#define MY_DISP_BUF_LEN    ((ST7789_LCD_WIDTH * ST7789_LCD_HEIGHT) / 10)
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);
static void disp_flush(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_display_t* disp = NULL;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /* Initialize low-level driver of used display */
    disp_init();

    /* Create main display */
    disp = lv_display_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);

    /* Create buffer to be used by display */
    static lv_color_t disp_buf_1[MY_DISP_BUF_LEN];
    //static lv_color_t disp_buf_2[MY_DISP_BUF_LEN];
    lv_display_set_buffers(disp, disp_buf_1, NULL, sizeof(disp_buf_1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    /* Register callbacks. */
    lv_display_set_flush_cb(disp, disp_flush);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    ST7789_GPIO_Init();
    ST7789_Init();
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

void disp_flush_ready_cb(void)
{
    ST7789_Async_TxFinished_cb();
    lv_display_flush_ready(disp);
}

/*Flush the content of the internal buffer the specific area on the display.
 *`px_map` contains the rendered image as raw pixel map and it should be copied to `area` on the display.
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_display_flush_ready()' has to be called when it's finished.*/
static void disp_flush(lv_display_t* disp_drv, const lv_area_t* area, uint8_t* colors)
{
    if(disp_flush_enabled) {
        ST7789_FillArea_Async(area->x1, area->y1, area->x2, area->y2, (uint16_t *)colors);
    }
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
