/**
 * @file GUI_StatusBar.h
 * @author Maksym T. (torgunakovmaks2@gmail.com)
 * @brief Module manages status bar.
 * This element contains all needed operational info. Is always visible accros all the screens / pages / etc.
 * 
 * @version 0.1 @date 2024-11-27
 *  - Status bar created. Only current time is updated.
 * 
 * 
 * @todo Change text color [black <--> white] based on the background color
 * @todo 
 * 
 */

#ifndef _GUI_STATUSBAR_H_
#define _GUI_STATUSBAR_H_

#include "sys.h"
#include "lvgl/lvgl.h"
/*=================================================================*/


/*=================================================================*/

/**
 * @brief Create object of the status bar.
 * Can create only one object at a time
 * 
 */
void GUI_StatusBar_Create();

/**
 * @brief Updates information located in the status bar.
 * Should be called periodically. Proposal is to place it inside lvgl_timer
 * 
 * @link https://docs.lvgl.io/9.1/overview/timer.html
 * 
 * @example 
 * lv_timer_t* timer = lv_timer_create(GUI_StatusBar_Update_Cyclic_10s, 500, &user_data);
 * lv_timer_enable(timer)
 * 
 */
void GUI_StatusBar_Update_Cyclic_1s(void);
/*=================================================================*/

#endif //_GUI_STATUSBAR_H_