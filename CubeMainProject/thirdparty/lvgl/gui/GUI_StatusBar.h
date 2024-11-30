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
/*=================================================================*/

#endif //_GUI_STATUSBAR_H_