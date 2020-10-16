#pragma once
#include <stdint.h>
#include "FRDM.h"
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef uint8_t pin_t;
typedef struct {
	uint8_t size;
} Message_t; //uint8 is temporary
typedef enum { RIGHT, LEFT } direction_t;


typedef uint_fast8_t encoder_id;
typedef enum { IDDLE_ANIMATION, ASK_PIN_ANIMATION, ACCESS_ANIMATION, OPEN_ANIMATION, USERS_ANIMATION, BRIGHTNESS_ANIMATION,INVALID_ID_ANIMATION, INVALID_PIN_ANIMATION,BLOCKED_ANIMATION
} animation_t;
//Claramente no vamos a anecesitar una animacion de cada uno, pero por las dudas le metiel define
/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void PVInit(void);
/**
 * @brief dispSendChar: sends to the selected 7 seg the character:
 * @param ch : character to be sent coded in ascii.
 * seven_seg_module: numer of seven segment, it ranges from 0 to 3
 * @return bool, false if any paramter was invalid
 *
 */
bool PVdispSendChar(char ch, uint8_t seven_seg_module);
void dispInit(void);

/**
 * @brief dispShift: shifts the msg in screen one row to the "direction"
 * @params direction:see the direction_t for the modes.
 */
void PVdispShift(direction_t direction);
/**
 * @brief dispClear: Clears the display, pretty self explanatory.
 *
 */
void PVdispClear(void);


/**
 * @brief dispBrightness: Changes the brightness of all displays.
 * @params brightness: the brightness value to be set. Lives between 0<b<100
 *
 */
void PVdispBrightness(uint8_t brightness);

void PVIncreaseBrightness(void);
void PVDecreaseBrightness(void);
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef uint8_t event_t;

typedef struct
{
    pin_t pin_A;
    pin_t pin_B;
    bool event_flag;
    event_t event_queue[20];
    uint8_t in_pointer;
    uint8_t out_pointer;
} encoder_state_t;

typedef uint8_t encoder_id;
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define HIGH 1
#define LOW 0

#define EVENT_AVB 1     //New event available
#define EVENT_NOT_AVB 0 //Nothing new to retrieve

#define LEFT_TURN 0
#define RIGHT_TURN 1
#define BUTTON_PRESS 2

 //Add new encoder
encoder_id PVencoder_register(void);
//Init encoder driver
void PVencoder_init(void);
//Update encoders
void PVencoder_update(void);
//Reports if there is an event available
bool PVencoder_event_avb(encoder_id id);
//Adds a new a event to the queue
void PVencoder_add_new_event(encoder_id id, event_t ev);
//Pops the last event in the queue
event_t PVencoder_pop_event(encoder_id id);

void PVAnimation(uint8_t animation, bool activate);
//Le mete una animacion u otra dependiendo el valor de animation
void PVIncreaseBrightness(void);
void PVDecreaseBrightness(void);