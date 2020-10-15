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
typedef uint8_t encoder_id;

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define EVENT_AVB 1     //New event available
#define EVENT_NOT_AVB 0 //Nothing new to retrieve

//EVENTS
#define LEFT_TURN 0
#define RIGHT_TURN 1

//Add new encoder
encoder_id EncoderRegister(pin_t pin_A, pin_t pin_B);
//Init encoder driver
void EncoderInit(void);
//Reports if there is an event available
bool EncoderEventAVB(encoder_id id);
//Pops the last event in the queue
event_t EncoderPopEvent(encoder_id id);
