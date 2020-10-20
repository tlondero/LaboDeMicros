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
#include <stdint.h>
#include "gpio.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



#define BUTTON_REFRESH_PERIOD 100
//THIS TRESHOLDS ARE NOT IN MS
//THE VALUE IN MS IS:  THRESHOLD_XXX * BUTTON REFRESH PERIOD
#define HIGH_WHEN_PRESSED 1
#define LOW_WHEN_PRESSED 0


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
	SKP, //Short key press  //Finisher of the ev queue
	EOQ
};

typedef uint8_t ButtonEvent ;//instant events such as PRESS and RELEASE


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief  Initializes buttons:
 * Starts initializing a timer, all the buttons share the one timer.
 * Creates one button and retunrs the id.
 *
 * @param
 * pin : kinetis pin for switch.
 * mode: mode of the switch, input, input pullup or input pulldown
 * @return  the ID for the switch -1 if the you reached the max switches
 */

int8_t ButtonInit(pin_t pin ,uint8_t mode, uint8_t active_low_or_high);

/**
 * @brief  ButtonSetIRQ: if you desire to use a dedicated interruption
 * this function allows you to set the type of interruption and the callback
 * to be called when the interruption occurs.
 *
 * @param
 * id : ID of the button.
 * IRQmode: mode of the  Interruption,
 *
    GPIO_IRQ_MODE_DISABLE = 0,
	GPIO_IRQ_MODE_LOW_STATE = 8,
	GPIO_IRQ_MODE_HIGH_STATE = 12,
    GPIO_IRQ_MODE_RISING_EDGE = 9,
    GPIO_IRQ_MODE_FALLING_EDGE = 10,
    GPIO_IRQ_MODE_BOTH_EDGES = 11,
    fcallback: Function to be called when the button is pressed
 */

void ButtonSetIRQ(int8_t id, uint8_t IRQ_mode,pinIrqFun_t fcallback);


/**
 * @brief  getter for the button events.
 *
 * @return EVENT pointer of the Button, check the enum section for the events.
 * The EOQ Refers to the tail of the ev queue
 */

const ButtonEvent * ButtonGetEvent(int8_t id);
bool ButtonCheckEvent(int8_t id);

/*******************************************************************************
 ******************************************************************************/

#endif /* BUTTON_H_ */

