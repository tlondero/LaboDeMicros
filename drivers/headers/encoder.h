/***************************************************************************/ /**
  @file     encoder.h
  @brief    Rotary encoder driver
  @author   MAGT
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define HIGH 1
#define LOW 0

#define FINITE 1
#define INFINITE 0

#define CLOCKWISE 1
#define COUNTER_CLOCKWISE 0

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
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

//Init encoder driver
void encoder_init(void);
//Update encoders
void encoder_update(void);
//Add new encoder
encoder_id encoder_register(pin_t pin_A, pin_t pin_B);
//Set max and min
void encoder_set_maxmin(encoder_id id, int16_t min, int16_t max);
//Set the current encoder position as a relative zero
void encoder_reset_cnt(encoder_id id);
//Get the current count
uint32_t encoder_get_count(encoder_id id);

