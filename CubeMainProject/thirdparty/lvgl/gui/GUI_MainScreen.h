/**
 * @file GUI_MainScreen.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-05
 * 
 * @copyright Copyright (c) 2024
 * 
 * @todo Send UpdateDateAndTime notification only if this screen is active.
 *      Try to bound update time and date not to labels but to the screen directly.
 *      If screen != NULL or is active ==> send notification. [To not hold pointers to the labels in ram]. 
 */

#ifndef _GUI_MAINSCREEN_H_
#define _GUI_MAINSCREEN_H_

#include "sys.h"
#include "lvgl/lvgl.h"

/**
 * @brief Create main screen
 *
 * @return lv_obj_t* - Pointer on the lv_screen.
 */
lv_obj_t* GUI_MainScreen_Create(void);

/**
 * @brief Notify system that the label with time should be updated.
 * Should be called at least every 1 second.
 *
 */
void GUI_MainScreen_UpdateDateAndTime_Notify(void);

#endif //_GUI_MAINSCREEN_H_
