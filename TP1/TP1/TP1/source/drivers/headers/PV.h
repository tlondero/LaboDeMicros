/***************************************************************************//**
 @file     PV.h
 @brief
 @author   MAGT
 ******************************************************************************/

#ifndef _PV_H_
#define _PV_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#include "led.h"
#include "Button.h"
#include "encoder.h"
#include "Display.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum {
	ENC_RIGHT, ENC_LEFT, BTN_PRESS, BTN_RELEASE, BTN_LKP, BTN_SKP, NO_PV_EV
} PVEv_t;

typedef enum {
	PV_DISABLE, PV_LSTATE, PV_HSTATE, PV_REDGE, PV_FEDGE, PV_BEDGES, PV_CANT_MODES
} PVIRQMode_t;

typedef enum { PV_RIGHT, PV_LEFT } PVDirection_t;

typedef enum { IDDLE_ANIMATION, ASK_PIN_ANIMATION, ACCESS_ANIMATION, OPEN_ANIMATION, USERS_ANIMATION, BRIGHTNESS_ANIMATION,INVALID_ID_ANIMATION
} PVAnimation_t;

typedef uint8_t PVButton_t;
typedef encoder_id PVEncoder_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Init Buttons, Leds, Encoder and Display
 */
void PVInit(void);


/**
 * @brief Get encoder or button event
 */
PVEv_t PVGetEv(void);


/**
 * @brief Clear display
 */
void PVDisplayClear(void);

/**
 * @brief Sets button IQR
 */
bool PVButtonIRQ(uint8_t IRQ_mode, pinIrqFun_t fcallback) ;

/**
 * @brief Set brightness
 */
bool PVDisplaySetBright(uint8_t brightness);

/**
 * @brief Increase brightness
 */
bool PVIncreaseBrightness(void);

/**
 * @brief Decrease brightness
 */
bool PVDecreaseBrightness(void);

/**
 * @brief dispSendChar: sends to the selected 7 seg the character:
 * @param ch : character to be sent coded in ascii.
 * seven_seg_module: numer of seven segment, it ranges from 0 to 3
 * @return bool, false if any paramter was invalid
 *
 */
bool PVDisplaySendChar(char ch, uint8_t seven_seg_module);

/**
 * @brief dispShift: shifts the msg in screen one row to the "direction"
 * @params direction:see the PVDirection for the modes.
 */
bool PVDisplayShift(PVDirection_t direction);

/**
 * @brief 
 */
bool PVAnimation(PVAnimation_t animation, bool activate);

/*******************************************************************************
 ******************************************************************************/

#endif // _PV_H_
