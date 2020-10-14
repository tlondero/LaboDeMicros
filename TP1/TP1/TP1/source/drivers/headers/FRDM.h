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

enum { RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, CANT_COLORS };

enum {R, G, B, NO_LED};

typedef uint8_t FRDMButton;
typedef uint8_t FRDMButtonEv;

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
FRDMButtonEv FRDMButtonGetEv(uint8_t button);

/**
 * @brief Turns on led
 * @param Color
 */ 
void FRDMLedColor(uint8_t color);

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
 * @brief Sets RGB brightnes value
 */
bool FRDMLedRGB(uint8_t r, uint8_t g, uint8_t b);


/**
 * @brief Set Led Bright
 */
bool FRDMLedBright(uint8_t led, uint8_t value);

/**
 * @brief Set Led Fade
 */
bool FRDMLedFade(uint8_t led, uint8_t value);

/**
 * @brief Set Led DT
 */
bool FRDMLedDt(uint8_t led, uint8_t value);

/**
 * @brief Set Led Flas time
 */
bool FRDMLedFlash(uint8_t led, uint8_t value);

/**
 * @brief Set Led Period
 */
bool FRDMLedPeriod(uint8_t led, uint8_t value);

/*******************************************************************************
 ******************************************************************************/

#endif // _FRDM_H_
