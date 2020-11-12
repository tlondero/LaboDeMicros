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


#define MAX_BUTTONS 15
#define BUTTON_REFRESH_PERIOD 100
//THIS TRESHOLDS ARE NOT IN MS
//THE VALUE IN MS IS:  THRESHOLD_XXX * BUTTON REFRESH PERIOD

#define VISIBLE_LEDS_EVENTS 0 //te va  a tirar repetido el evento pero lo vas a ver

#define LKP_THRESHOLD 50

#define TYPEMATIC_THRESHOLD 15
#define TYPEMATIC_PERIOD 3

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
//EVENTS:
enum{NOT_EN,// The button you asked for wasnt enabled
	NO_EV,//No event was detected by the button
	PRESS,//Button was pressed
	RELEASE,//button was released
	LKP,// Long Key Press
	SKP
};

typedef uint8_t Button_Event;
typedef struct{
	uint8_t ev_state;
	uint8_t pin_state;
	uint8_t prev_pin_state;
	pin_t pin;
	bool enable;
	bool lkp; //TRUE is LKP FALSE is TYPEMATTIC
}Btn;
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
 * @return  the ID for the switch -1 if the you reached the max switches
 */

int8_t initButton(pin_t pin ,uint8_t mode);
/**
 * @brief  getter for the button events.
 *
 * @return EVENT of the Button, check the enum section for the events.
 */
Button_Event getButtonEvent(int8_t id);

/**
 * @brief  lets you choose working mode, LKP or Typematic.
 *
 * @param
 * id: Id of the button.
 * mode: True for LKP  or False for  Typematic
 */

void LKP_or_Typematic_mode(int8_t id, bool mode);

/*******************************************************************************
 ******************************************************************************/

#endif /* BUTTON_H_ */

