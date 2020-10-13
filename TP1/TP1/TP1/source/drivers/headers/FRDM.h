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

typedef uint8_t FRDMButton;
typedef uint8_t FRDMButtonEv;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Inits FDRM Buttons
 * @param Recieves which button is going to be used (SW2 or SW3)
 */
FRDMButton FRDMButtonInit(uint8_t button);


/**
 * @brief Returns the event of the button
 * @param 
 */
FRDMButtonEv FRDMButtonGetEv(uint8_t button);


/**
 * @brief Init Led
 */ 
void FRDMLedInit(void);


/**
 * @brief Turns on led
 * @param Color
 */ 
void FRDMLedColor(uint8_t color);


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


/*******************************************************************************
 ******************************************************************************/

#endif // _FRDM_H_
