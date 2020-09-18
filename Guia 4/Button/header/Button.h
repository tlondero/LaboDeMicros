/***************************************************************************//**
  @file     Button.h
  @brief    Simple  driver for buttons
  @author   MAGT
 ******************************************************************************/

#ifndef BUTTON_H_
#define BUTTON_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdbool.h>
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
typedef uint8_t Button_Event;
typedef struct{
	uint8_t ev_state;
	uint8_t pin_state;
	uint8_t prev_pin_state;
	pin_t pin;
	bool enable;
	bool lkp;
}Btn;

#define MAX_BUTTONS 15
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
//EVENTS:
enum{NOT_EN,// The button you asked for wasnt enabled
	NO_EV,//No event was detected by the button
	PRESS,//Button was pressed
	RELEASE,//button was released
	LKP,// Long Key Press
};

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief  initializes buttons.
 *
 * @param
 * pin : kinetis pin for switch.
 * mode: mode of the switch, input, input pullup or input pulldown
 * LKP_or_Typemattic: true if you want LKP mode, 0 if you want TYPEMATTIC:
 * TYPEMATTIC-> recieve press event if you  keep the button pressed.
 * LKP-> get Long key pressed event if you keep pressing the button.
 * @return  the ID for the switch -1 if the you reached the max switches
 */

int8_t initButton(pin_t pin ,uint8_t mode, bool LKP_or_Typemattic);
/**
 * @brief  getter for the button events.
 *
 * @return EVENT of the Button, check the enum section for the events.
 */
Button_Event getButtonEvent(int8_t id);

/*******************************************************************************
 ******************************************************************************/

#endif /* BUTTON_H_ */
¬
