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

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define PWM_NO_SPACE -1
#define PWM_BAD_OP 0
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Initializes AND STARTS a PWM signal.
 * @param pin the pin where the pwm signal is wanted. THE PIN HAS TO HAVE
 * A FTM ALTERNATIVE!
 * @param freq the desired frequency of the signal.
 * @param dt the duty cycle of the desired signal.
 * @initial_state the initial state of the signal.
 * @returns pwm_id the id of the initialized pwm.
 */
int8_t PWMInitSignal(uint8_t pin, double freq, double dt, uint8_t initial_state);
/**
 * @brief Resumes a stopped PWM signal.
 * @param pwm_id pwm id given by init_pwm
 */
void PWMStartSignal(int8_t pwm_id);
/**
 * @brief Stops a pin from outputting a PWM signal.
 * @param pwm_id the id of the queried pwm.
 * @param final_state the electrical state of the pin when the pwm is stopped.
 */
void PWMStopSignal(int8_t pwm_id, uint8_t final_state);
/**
 * @brief Destroys a signal and frees space
 * @param pwm_id the id of the pwm signal
 * @param final_state the final electrical state of the pin
 */
void PWMDestroySignal(uint8_t pwm_id, uint8_t final_state);
/**
 * @brief Sets a new frecuency
 * @param pwm_id the id of the pwm signal
 * @new_freq the new frecuency
 * @returns BAD_OP when the operation could not be satisfied
 * else returns 1.
 */
uint8_t PWMSetFrequency(uint8_t pwm_id, double new_freq);
/**
 * @brief Sets a new duty cycle
 * @param pwm_id the id of the pwm signal
 * @param new_dt the new duty cycle
 * @returns BAD_OP when the operation could not be satisfied
 * else returns 1.
 */
uint8_t PWMSetDT(uint8_t pwm_id, double new_dt);
/**
 * @brief increments the duty cycle of the signal by a certain quantity
 * @param pwm_id the id of the pwm signal
 * @param deltaDT the increment quantity
 * @returns BAD_OP when the operation could not be satisfied
 * else returns 1.
 */
uint8_t PWMIncrementDT(uint8_t pwm_id, double deltaDT);	//TODO
/**
 * @brief decrements the duty cycle of the signal by a certain quantity
 * @param pwm_id the id of the pwm signal
 * @param deltaDT the decrement quantity
 * @returns BAD_OP when the operation could not be satisfied
 * else returns 1.
 */
uint8_t PWMDecrementDT(uint8_t pwm_id, double deltaDT);	//TODO


/*******************************************************************************
 ******************************************************************************/

#endif // _LED_H_
