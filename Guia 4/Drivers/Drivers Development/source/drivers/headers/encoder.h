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

//Add new encoder
encoder_id encoder_register(pin_t pin_A, pin_t pin_B);
//Init encoder driver
void encoder_init(void);
//Update encoders
void encoder_update(void);
//Reports if there is an event available
bool encoder_event_avb(encoder_id id);
//Adds a new a event to the queue
void encoder_add_new_event(encoder_id id, event_t ev);
//Pops the last event in the queue
event_t encoder_pop_event(encoder_id id);
