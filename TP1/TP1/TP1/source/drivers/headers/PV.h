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

#include "led_old.h"
//#include "led.h"

#include "Button.h"
#include "encoder.h"
#include "Display.h"
#include "../../app/headers/FEData.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define MAX_MESS_LEN 	100

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum {
	ENC_RIGHT, ENC_LEFT, BTN_PRESS, BTN_RELEASE, BTN_LKP, BTN_SKP, NO_PV_EV
} PVEv_t;

typedef enum {
	PV_DISABLE, PV_LSTATE, PV_HSTATE, PV_REDGE, PV_FEDGE, PV_BEDGES, PV_CANT_MODES
} PVIRQMode_t;

typedef enum {
	PV_LED_1, PV_LED_2, PV_LED_3, PV_LED_ALL, PV_CANT_COLORS
} PVLed_t;

typedef enum {
	ON_ST_1, ON_ST_2, ON_ST_3, ON_ST_OFF
} PVStatus_t;

typedef enum { PV_RIGHT, PV_LEFT, PV_NODIR } PVDirection_t;

//typedef enum { IDDLE_ANIMATION, ASK_PIN_ANIMATION, ACCESS_ANIMATION, OPEN_ANIMATION, USERS_ANIMATION, BRIGHTNESS_ANIMATION,INVALID_ID_ANIMATION
//} PVAnimation_t;

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
bool PVInit(void);

void PVSuscribeEvent(PVEv_t ev, bool state);

bool PVCheckEvent(void);

/**
 * @brief Sets button IQR
 */
bool PVButtonIRQ(uint8_t IRQ_mode, pinIrqFun_t fcallback) ;

/**
 * @brief Poll LED and pwm
 */
void PVLedPoll(void);

/**
 * @brief Clear display
 */
void PVDisplayClear(void);

bool PVMarquesina(char * mess);

bool PVDisplaySendChar(char ch, uint8_t seven_seg_module);

/**
 * @brief Get encoder or button event
 */
PVEv_t PVGetEv(void);

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
 * @brief dispShift: shifts the msg in screen one row to the "direction"
 * @params direction:see the PVDirection for the modes.
 */
bool PVDisplaySetShift(PVDirection_t direction);

bool PVDisplaySetTime(uint32_t time);

bool PVDispMessOn(void);

bool PVDispManualShift(PVDirection_t direction, uint8_t cant);

/**
 * @brief
 */
bool PVAnimation(animation_t animation, bool activate);

/**
 * @brief LED Set brightnes
 */
bool PVLedSetBright(PVLed_t led, uint8_t value);

/**
 * @brief LED Set fade time
 */
bool PVLedSetFade(PVLed_t led, uint8_t value);

/**
 * @brief LED Set DT %
 */
bool PVLedSetDt(PVLed_t led, uint8_t value);

/**
 * @brief LED  Set amounts of flashes (0 = forever)
 */
bool PVLedSetFlash(PVLed_t led, uint8_t value);

/**
 * @brief LED Set time on
 */
bool PVLedSetPeriod(PVLed_t led, uint8_t value);

/**
 * @brief LED  Set flash period
 */
bool PVLedSetTime(PVLed_t led, uint8_t value);

/**
 * @brief LED  Set flash ON
 */
bool PVLedFlash(PVLed_t led, uint8_t value);

/**
 * @brief LED COLOR SET
 */
bool PVLedOn(PVLed_t led);

/**
 * @brief Turn off ALL LED (R G B)
 */
void PVLedOff(PVLed_t led);

void PVStatusLedSelect(PVStatus_t led, bool state);

/*******************************************************************************
 ******************************************************************************/

#endif // _PV_H_
