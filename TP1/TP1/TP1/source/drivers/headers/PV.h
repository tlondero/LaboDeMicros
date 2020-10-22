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

/**
 * @brief Suscribe to an ivent
 */
void PVSuscribeEvent(PVEv_t ev, bool state);

/**
 * @brief Check if there is an event
 */
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

/**
 * @brief Roll message on disp
 * @param *mess: message
 *        time_per_char: time to shift 1 digit
 */
bool PVMarquesina(char *mess, uint32_t time_per_char);

/**
 * @brief Send char to display
 * @param ch: char
 *        seven_seg_module: 0-3 display selected
 */
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
 * @brief Increase brightness in delta 
 */
bool PVIncreaseBrightness(void);

/**
 * @brief Decrease brightness in delta 
 */
bool PVDecreaseBrightness(void);

/**
 * @brief Returns brightness value
 */
int8_t PVGetBrightness(void);

/**
 * @brief Set brightness value
 */
bool PVSetDeltaBright(uint8_t br);

/**
 * @brief Set brightness delta value
 */
int8_t PVGetDeltaBright(void);

/**
 * @brief dispShift: shifts the msg in screen one row to the "direction"
 * @params direction:see the PVDirection for the modes.
 */
bool PVDisplaySetShift(PVDirection_t direction);


bool PVDisplaySetTime(uint32_t time);

/**
 * @brief Start rolling message
 */
bool PVDispMessOn(void);

/**
 * @brief Shift cracters in display
 * @param direction: right/left
 * 		  cant: number of shifts
 */
bool PVDispManualShift(PVDirection_t direction, uint8_t cant);

/**
 * @brief
 */
bool PVAnimation(animation_t animation, bool activate);

/**
 * @brief LED Set brightnes
 * @param led: the led selected
 * 		  value: bright value
 */
bool PVLedSetBright(PVLed_t led, uint8_t value);

/**
 * @brief LED Set fade time
 * @param led: the led selected
 * 		  value: fade time
 */
bool PVLedSetFade(PVLed_t led, uint8_t value);

/**
 * @brief LED Set DT %
 * @param led: the led selected
 * 		  value: dt
 */
bool PVLedSetDt(PVLed_t led, uint8_t value);

/**
 * @brief LED  Set amounts of flashes 
 * @param led: the led selected
 * 		  value: flash times (0 = forever)
 */
bool PVLedSetFlash(PVLed_t led, uint8_t value);

/**
 * @brief LED Set period time
 * @param led: the led selected
 * 		  value: period
 */
bool PVLedSetPeriod(PVLed_t led, uint8_t value);

/**
 * @brief LED  Set flash period
 * @param led: the led selected
 * 		  value: time
 */
bool PVLedSetTime(PVLed_t led, uint8_t value);

/**
 * @brief LED  Set flash ON
 * @param led: the led selected
 * 		  value: flash times
 */
bool PVLedFlash(PVLed_t led, uint8_t value);

/**
 * @brief Turns on led
 * @param led: the led selected
 */
bool PVLedOn(PVLed_t led);

/**
 * @brief Turn off ALL LED (R G B)
 */
void PVLedOff(PVLed_t led);

/**
 * @brief Select pv red led status
 * @param led: the led selected
 * 		  state: on=true - off=false
 */
void PVStatusLedSelect(PVStatus_t led, bool state);

/*******************************************************************************
 ******************************************************************************/

#endif // _PV_H_
