/***************************************************************************/ /**
  @file     FRDM.h
  @brief    
  @author   MAGT
 ******************************************************************************/

#ifndef _FRDM_H_
#define _FRDM_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#include "led_old.h"
#include "Button.h"
#include "gpio.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define		BUTTON_SW2		PORTNUM2PIN(PC,6)
#define		BUTTON_SW3		PORTNUM2PIN(PA,4)

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

enum {
	RED, YELLOW, GREEN, BLUE, CYAN, PURPLE, WHITE, CANT_COLORS
};

typedef enum{
	PRESS_SW2, RELEASE_SW2, LKP_SW2, SKP_SW2, PRESS_SW3, RELEASE_SW3, LKP_SW3, SKP_SW3, NO_FRDM_EV
} FRDMEv_t;

enum {
	BT_DISABLE, BT_LSTATE, BT_HSTATE, BT_REDGE, BT_FEDGE, BT_BEDGES, BT_CANT_MODES
};

typedef uint8_t FRDMButton_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Init Buttons and Leds
 */
bool FRDMInit(void);

/**
 * @brief Returns the event of the button
 * @param button: button selected
 */
FRDMEv_t FRDMButtonGetEv(uint8_t button);

/**
 * @brief Set fade time
 * @param value: amount of fade time
 */
bool FRDMLedSetFade(uint8_t value);

/**
 * @brief Set DT %
 * @param value: amount of dt
 */
bool FRDMLedSetDt(uint8_t value);

/**
 * @brief Set amounts of flashes (0 = forever)
 * @param value: flash times
 */
bool FRDMLedSetFlash(uint8_t value);

/**
 * @brief Set flash period
 * @param value: period time
 */
bool FRDMLedSetPeriod(uint8_t value);

/**
 * @brief Set flash ON
 * @param led: seledted led
 */
bool FRDMLedFlash(uint8_t led);

/**
 * @brief Set button IQR
 * @param button: selected button
 *		  IRQ_mode: action mode
 *        fcallback: callback
 */
bool FRDMButtonIRQ(uint8_t button, uint8_t IRQ_mode, pinIrqFun_t fcallback);

/**
 * @brief Turn off all leds (R G B)
 */
void FRDMLedOff(void);

/**
 * @brief Poll led
 */
void FRDMLedPoll(void);

/**
 * @brief Led on time
 * @param value: led time
 */
bool FRDMLedSetTime(uint8_t value);

/**
 * @brief Toggle On/Off whit last color set
 */
void FRDMToggleOnOff(void);

/**
 * @brief Turns on a color
 * @param color: color desired
 */
void FRDMLedColor(uint8_t color);

/**
 * @brief Check if there is an event
 */
bool FRDMCheckEvent(void);

/**
 * @brief Get button event
 */
FRDMEv_t FRDMGetEv(void);

/**
 * @brief Suscribe to an ivent
 */
void FRDMSuscribeEvent(FRDMEv_t ev, bool state);


/*******************************************************************************
 ******************************************************************************/

#endif // _FRDM_H_
