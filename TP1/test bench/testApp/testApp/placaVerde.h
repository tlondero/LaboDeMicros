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
typedef struct
{
    pin_t pin_A;
    pin_t pin_B;
    int16_t max;
    int16_t min;
    int16_t count;
    uint8_t current_state_A : 1;
    uint8_t finite_count : 1; // Allow Inifinte
    uint8_t current_state_B : 1;
    uint8_t prev_state_A : 1;
    uint8_t prev_state_B : 1;
    uint8_t clockwise : 1; //Increase counter in the clockwise direction
    uint8_t unused : 1;

} encoder_state_t;

typedef uint_fast8_t encoder_id;

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

//Init encoder driver
void PVencoder_init(void);
//Update encoders
encoder_id PVencoder_register(void);
//Set max and min
int8_t PVencoder_pop_event(encoder_id id);
//Set the current encoder position as a relative zero
bool PVencoder_is_there_ev(encoder_id id);
//Get the current count
