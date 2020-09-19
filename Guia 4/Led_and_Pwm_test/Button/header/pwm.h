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
#include "header/gpio.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define MAX_PWMS 16
#define UNAVAILABLE_SPACE -1
#define DEVELOPMENT_MODE 1
#define PWM_TIMEBASE 1 //ms
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct {
	uint32_t freq;
	uint8_t pwm_id;
	uint32_t dt;
	uint8_t pin;
	uint8_t queried;
	uint32_t last_updated;
}pwm_t;
/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Initializes pwm driver requesting a timer.
 */
void init_pwm_driver(void);
/**
 * @brief Asks for an id, given no space, outputs UNAVAILABLE_SPACE.
 * @returns pwm signal id
 */
int8_t get_pwm_id(void);
/**
 * @brief Initializes a pwm signal
 * @param pin the pin where the pwm signal is wanted.
 * @returns pwm_id the id of the initialized pwm.
 */
int8_t init_pwm(pin_t pin);
/**
 * @brief Makes an initialized pwm output a PWM signal.
 * @param pwm_id pwm id given by init_pwm
 * @param freq the desired pwm frequency.
 * @param dt the desired pwm duty cycle.
 */
void query(int8_t pwm_id, uint32_t freq, uint32_t dt, uint8_t initial_state);
/**
 * @brief Stops a pin from outputting a PWM signal.
 * @param pwm_id the id of the queried pwm.
 */
void unquery(int8_t id, uint8_t final_state);
/**
 * @brief polls whether some pin has to change state given
 * the queried pwms.
 */
void poll_pwms(void);
/*******************************************************************************
 ******************************************************************************/

#endif // _LED_H_
