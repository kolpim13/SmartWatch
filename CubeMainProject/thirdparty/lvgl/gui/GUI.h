/**
 * @file GUI.h
 * @author Maksym T. (torgunakovmaks2@gmail.com)
 * @brief
 * @version 0.1 @date 2024-06-28
 * 
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _LVGL_GUI_H_
#define _LVGL_GUI_H_

#include "lvgl/lvgl.h"
#include "GUI_StatusBar.h"
#include "GUI_MainPage.h"
#include "GUI_SettingsPage.h"
/*=================================================================*/

void GUI_Init(void);
/*=================================================================*/

/* LVGL TIMERS */
void timer_statusBar_update(lv_timer_t* timer);
/*=================================================================*/


#endif /* _LVGL_GUI_H_ */
