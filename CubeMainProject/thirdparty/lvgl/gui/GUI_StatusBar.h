/**
 * @file GUI_StatusBar.h
 * @author Maksym T. (torgunakovmaks2@gmail.com)
 * @brief Module manages status bar.
 * This element contains all needed operational info. Is always visible accros all the screens / pages / etc.
 * 
 * @version 0.1 @date 2024-11-27
 *  - Status bar created. Only current time is updated.
 * 
 * @version 0.2 @date 2024-11-30
 *  - Logic of timer to update status bar was moved inside this file. 
 * 
 * @version 0.3 @date 2024-12-12
 *  - Interface "GUI_StatusBar_Update". It will update status bar 
 * on the next cycle of lvgl main handler.
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
void GUI_StatusBar_Create(void);

/**
 * @brief Function is called to update status bar immediately
 * 
 */
void GUI_StatusBar_Update(void);
/*=================================================================*/

#endif //_GUI_STATUSBAR_H_