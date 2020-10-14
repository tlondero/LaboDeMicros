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

enum {
	ENC_RIGHT, ENC_LEFT, BTN_PRESS, BTN_RELEASE, BTN_LKP, BTN_SKP, NO_PV_EV
};

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef uint8_t PVEv;

typedef uint8_t PVButton;
typedef encoder_id PVEncoder;

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
PVEv PVGetEv(void);


/**
 * @brief Clear display
 */
void PVDisplayClear(void);

/**
 * @brief Set brightness
 */
bool PVDisplaySetBright(uint8_t brightness);

/**
 * @brief Increase brightness
 */
bool PVDisplayIncBright(void);

/**
 * @brief Decrease brightness
 */
bool PVDisplayDecBright(void);


/*******************************************************************************
 ******************************************************************************/

#endif // _PV_H_
