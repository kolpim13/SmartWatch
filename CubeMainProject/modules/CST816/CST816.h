/**
 * @file CST816.h
 * @author Maksym T. (torgunakovmaks2@gmail.com)
 * @brief Current HW version of the chip supports only single finger event and gestures.
 * 
 * @version 0.1 @date 2024-06-27
 * @version 0.2 @date 2024-12-05
 *  - Interface "CST816_GetNumOfFingers" not used. Its functionality moved to "CST816_IsTouch"
 *  - Interface "CST816_Touch_and_AxisXY" added.
 *  - Sleep mode added. [Not tested]
 * 
 * @todo
 * 1. Compare what is faster: CST816_IsTouch + CST816_GetAxis_XY or CST816_Touch_and_AxisXY.
 *  And what is exact difference.
 * 2. Test somehow sleep mode | wakeup.
 * 3. Add Low power mode: faster entering standby mode, lower frequency, etc.
 * 4. Add Some DBG / INFO information: Chip ID, Version, etc.
 */

#include "sys.h"

/* Address itself is 0x15.
 * But it assumes to be 0x2A for write and 0x2B for read operations.
 * Hal will add "1" to the address for read operations automatically. */
#define CST816T_ADDRESS         0x2A //0x15

#define CST816_OFFSET_X			20u
#define CST816_OFFSET_Y			20u

#define CST816_GESTURE_ID       0x01
#define CST816_FINGER_NUM       0x02    /* This HW do not recognize more than 1 finger. */
#define CST816_XPOS_H           0x03
#define CST816_XPOS_L           0x04
#define CST816_YPOS_H           0x05
#define CST816_YPOS_L           0x06
#define CST816_POWER_MODE       0xA5    /* 0x03 - sleep mode */
#define CST816_CHIP_ID          0xA7
#define CST816_PROJ_ID          0xA8
#define CST816_FW_VERSION       0xA9
#define CST816_FACTORY_ID       0xAA
#define CST816_SLEEP_MODE       0xE5
#define CST816_NOR_SCAN_PERIOD  0xEE    /* [1 - 30] unit: 10 ms.*/
#define CST816_AUTO_SLEEP_TIME  0xF9
#define CST816_IRQ_CTL          0xFA
#define CST816_LONG_PRESS_TICK  0xEB
#define CST816_MOTION_MASK      0xEC
#define CST816_DIS_AUTOSLEEP    0xFE
/*=================================================================*/

#define CST816_RST_PORT         GPIOB
#define CST816_RST_PIN          GPIO_PIN_4

#define CST816_INT_PORT         GPIOA
#define CST816_INT_PIN          GPIO_PIN_15
/*=================================================================*/

#define CST816_RST_Set()        HAL_GPIO_WritePin(CST816_RST_PORT, CST816_RST_PIN, GPIO_PIN_SET)
#define CST816_RST_Reset()      HAL_GPIO_WritePin(CST816_RST_PORT, CST816_RST_PIN, GPIO_PIN_RESET)
/*=================================================================*/

void CST816_GPIO_Init(void);
void CST816_Reset(void);
void CST816_Init(void);

/**
 * @brief Reads number of fingers present on the screen.
 * If more than 0 --> touch is present
 * 
 * @return bool - if there is a touch. 
 */
bool CST816_IsTouch(void);

/**
 * @brief Read position of the touch on the screen
 * 
 * @param x [out]
 * @param y [out]
 */
void CST816_GetAxis_XY(uint16_t* x, uint16_t* y);

/**
 * @brief Check if there is a touch present.
 * If yes --> reads its coordinates.
 * 
 * @param x [out]
 * @param y [out]
 * @return bool. True - touch is present; False - otherwise. 
 */
bool CST816_Touch_and_AxisXY(uint16_t* x, uint16_t* y);

/**
 * @brief To be tested somehow.
 * 
 */
void CST816_Sleep_Enter(void);

/**
 * @brief TBD
 * 
 */
void CST816_Sleep_Wakeup(void);
