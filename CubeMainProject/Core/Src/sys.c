#include "sys.h"

#include "ST7789.h"

#include "lvgl/lvgl.h"
#include "lvgl/lv_port_disp.h"
#include "lvgl/lv_port_indev.h"

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    /* LCD Callback */
    if (hspi == &hspi1)
    {
        ST7789_Async_TxFinished_cb();
        lv_display_flush_ready(disp_main_get());
    }
}
/*=================================================================*/
