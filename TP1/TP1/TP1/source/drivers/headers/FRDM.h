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

enum { BRIGHT, FADE, DT, FLASHTIME, PERIOD };

enum {R, G, B, NONE};

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
 * @brief Turns on led
 * @param Color
 */
bool FRDMButtonIRQ(uint8_t button, uint8_t IRQ_mode, pinIrqFun_t fcallback);

/**
 * @brief Turn off leds
 */
void FRDMLedOff(void);


/**
 * @brief Poll led and pwm
 */
void FRDMLedPoll(void);


/**
 * @brief Poll led and pwm
 */
bool FRDMLedRGB(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Poll led and pwm
 */
bool FRDMLedFlash(uint8_t port);


/**
 * @brief Poll led and pwm
 */
bool FRDMLedConfig(uint8_t led, uint8_t param, uint8_t value);

/*******************************************************************************
 ******************************************************************************/

#endif // _FRDM_H_
