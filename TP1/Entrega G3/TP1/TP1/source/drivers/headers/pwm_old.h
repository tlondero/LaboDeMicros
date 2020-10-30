/***************************************************************************//**
  @file     pwm.h
  @brief    PWM services
  @author   MAGT
 ******************************************************************************/

#ifndef _PWM_H_
#define _PWM_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "../headers/gpio.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define MAX_PWMS 16
#define UNAVAILABLE_SPACE -1
#define PWM_TIMEBASE 0.1 //ms

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Initializes pwm driver requesting a timer.
 */
void pwm_init_driver(void);
/**
 * @brief Asks for an id, given no space, outputs UNAVAILABLE_SPACE.
 * @returns pwm signal id
 */
int8_t pwm_get_id(void);
/**
 * @brief Initializes a pwm signal
 * @param pin the pin where the pwm signal is wanted.
 * @returns pwm_id the id of the initialized pwm.
 */
int8_t pwm_init_signal(pin_t pin);
/**
 * @brief Makes an initialized pwm output a PWM signal.
 * @param pwm_id pwm id given by init_pwm
 * @param freq the desired pwm frequency.
 * @param dt the desired pwm duty cycle.
 */
void pwm_query(int8_t pwm_id, uint32_t freq, uint32_t dt, uint8_t initial_state);
/**
 * @brief Stops a pin from outputting a PWM signal.
 * @param pwm_id the id of the queried pwm.
 */
void pwm_unquery(int8_t id, uint8_t final_state);
/**
 * @brief polls whether some pin has to change state given
 * the queried pwms.
 */
void pwm_poll(void);
/*******************************************************************************
 ******************************************************************************/

#endif // _LED_H_
