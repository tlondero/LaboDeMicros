/***************************************************************************//**
  @file     Display.h
  @brief
  @author   MAGT
 ******************************************************************************/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******/
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct{
	uint8_t size;
} Message_t; //uint8 is temporary

typedef enum{RIGHT,LEFT} direction_t;
/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/**
 * @brief dispInit: Initializes the display
 *
 */
void dispInit(void);



/**
 * @brief dispSendMsg: the idea should be to print the msg send by the user, the Message_t struct is under construction
 *
 */
void dispSendMsg(Message_t msg);

/**
 * @brief dispSendChar: sends to the selected 7 seg the character:
 * @param ch : character to be sent coded in ascii.
 * seven_seg_module: numer of seven segment, it ranges from 0 to 3
 *
 */
void dispSendChar(char ch, uint8_t seven_seg_module);


/**
 * @brief dispShift: shifts the msg in screen one row to the "direction"
 * @params direction:see the direction_t for the modes.
 */
void dispShift(direction_t direction);

/**
 * @brief dispClearAll: Clears ALL the display, pretty self explanatory.
 *
 */
void dispClearAll(void);

/**
 * @brief dispBrightness: Changes the brightness of all displays.
 * @params brightness: the brightness value to be set. Lives between 0<b<100
 *
 */
void dispBrightness(uint8_t brightness);

/*******************************************************************************
 ******************************************************************************/

#endif // _DISPLAY_H_
