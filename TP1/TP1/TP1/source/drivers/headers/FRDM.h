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


enum {
	R, G, B, NO_LED
};

enum {
	BT_DISABLE, BT_LSTATE, BT_HSTATE, BT_REDGE, BT_FEDGE, BT_BEDGES, BT_CANT_MODES
};

typedef uint8_t FRDMButton_t;
typedef uint8_t FRDMButtonEv_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Init Buttons and Leds
 */
void FRDMInit(void);

/**
 * @brief Returns the event of the button
 * @param 
 */
FRDMButtonEv_t FRDMButtonGetEv(uint8_t button);

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
void FRDMLedToggle(void);

/**
 * @brief Turns on a color
 */
void FRDMLedColor(uint8_t color);

/*
bool FRDMLedRGB(uint8_t r, uint8_t g, uint8_t b);
bool FRDMLedBright(uint8_t led, uint8_t value);
bool FRDMLedFade(uint8_t led, uint8_t value);
bool FRDMLedDt(uint8_t led, uint8_t value);
bool FRDMLedFlash(uint8_t led, uint8_t value);
bool FRDMLedPeriod(uint8_t led, uint8_t value);
*/


/*******************************************************************************
 ******************************************************************************/

#endif // _FRDM_H_
