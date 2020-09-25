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



#define LKP_THRESHOLD 50

#define TYPEMATIC_THRESHOLD 15
#define TYPEMATIC_PERIOD 3

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
//EVENTS:
enum{
	NO_EV,//No event was detected by the button
	PRESS,//Button was pressed
	RELEASE,//button was released
	LKP,// Long Key Press
	SKP
};

typedef struct{
	uint8_t inst_evs;//instant events such as PRESS and RELEASE
	uint8_t hold_evs;// hold events like LKP and SKP
} Button_Event ;

typedef struct{
	uint8_t pin_state;
	uint8_t prev_pin_state;
	pin_t pin;
	bool enable;
	bool newinfo; //flag for the callback to announce new information
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
 * @return  the ID for the switch -1 if the you reached the max switches
 */

int8_t initButton(pin_t pin ,uint8_t mode);

/**
 * @brief  setIRQ_button:
 *
 * @param
 * id : ID of the button.
 * mode: mode of the  Interruption,
 *
    GPIO_IRQ_MODE_DISABLE = 0,
	GPIO_IRQ_MODE_LOW_STATE = 8,
	GPIO_IRQ_MODE_HIGH_STATE = 12,
    GPIO_IRQ_MODE_RISING_EDGE = 9,
    GPIO_IRQ_MODE_FALLING_EDGE = 10,
    GPIO_IRQ_MODE_BOTH_EDGES = 11,
 */

void setIRQ_button(int8_t id, uint8_t IRQMode,pinIrqFun_t fcallback);


/**
 * @brief  getter for the button events.
 *
 * @return EVENT of the Button in the Button_ev struct, check the enum section for the events.
 */

Button_Event getButtonEvent(int8_t id);


/*******************************************************************************
 ******************************************************************************/

#endif /* BUTTON_H_ */

