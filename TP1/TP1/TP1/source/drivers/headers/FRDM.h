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

#include "led.h"
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
 */
FRDMEv_t FRDMButtonGetEv(uint8_t button);

/**
 * @brief Set fade time
 */
bool FRDMLedSetFade(uint8_t value);

/**
 * @brief Set DT %
 */
bool FRDMLedSetDt(uint8_t value);

/**
 * @brief Set amounts of flashes (0 = forever)
 */
bool FRDMLedSetFlash(uint8_t value);

/**
 * @brief Set flash period
 */
bool FRDMLedSetPeriod(uint8_t value);

/**
 * @brief Set flash ON
 */
bool FRDMLedFlash(uint8_t led);

/**
 * @brief Set button IQR
 */
bool FRDMButtonIRQ(uint8_t button, uint8_t IRQ_mode, pinIrqFun_t fcallback);

/**
 * @brief Turn off ALL leds (R G B)
 */
void FRDMLedOff(void);

/**
 * @brief Poll led and pwm
 */
void FRDMLedPoll(void);

/**
 * @brief Toggle On/Off whit last color set
 */
void FRDMToggleOnOff(void);

/**
 * @brief Turns on a color
 */
void FRDMLedColor(uint8_t color);


bool FRDMCheckEvent(void);

FRDMEv_t FRDMGetEv(void);

void FRDMSuscribeEvent(FRDMEv_t ev, bool state);


/*******************************************************************************
 ******************************************************************************/

#endif // _FRDM_H_
