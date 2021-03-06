/***************************************************************************//**
  @file     led.h
  @brief    LED services
  @author   MAGT
 ******************************************************************************/

#ifndef _LED_H_
#define _LED_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "header/led.h"
//#include "header/timer.h"
#include "header/pwm.h"
#include "header/gpio.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define STAY_FOREVER 0
#define NO_FADE 0
#define UNAVAILABLE_SPACE -1
#define MAX_LEDS 16
#define DEVELOPMENT_MODE 1

#define SET_LED_VALUES
#define SET_HIGH 1
#define SET_LOW 0
#define TOGGLE 2

#define PIN_MODE
#define TURNS_ON_WITH_1 0
#define TURNS_ON_WITH_0	1

#define LED_TIMEBASE 25
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct {
	uint32_t time_start;//time when the LED started waiting
	uint16_t time;		//time the LED will stay on the new state in ms. 0 means forever.
	uint16_t period;	//period for flashing LEDs in ms.
	uint16_t flashes;	//times the LED will flash in flashing mode. 0 means forever.
	uint16_t fade;		//time the LED will take to fully change states on a fade in ms. 0 means no fade.
	int8_t led_id; 	//ID of the initialized LED
	uint8_t led_pin;	//pin of the LED
	uint8_t led_pin_mode;//pin mode of the LED
	uint8_t brightness;	//brightness, values from 0 to 100 and has a 1:1 ratio with duty cycle
	uint8_t flashing;
}led_t;
/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void init_led_driver();
/**
 * @brief Initialize one LED
 * @param pin the pin which governs the LED
 * @param pin_mode whether the pin has to be pulled up or down
 * @param led_id the id given by get_id()
 */
int8_t init_led(pin_t pin, uint8_t pin_mode, int8_t led_id);
/**
 * @brief take out LED from LED array
 * @param led_id the id given by get_id()
 */
void destroy_led(int8_t led_id);
int8_t get_led_id();
/**
 * @brief configurate certain aspect of LED
 * @param led_id the id given by get_id()
 * @param xxxxx the aspect to configure
 */
void configure_brightness(int8_t led_id, uint8_t brightness);
void configure_time(int8_t led_id, uint16_t time);
void configure_period(int8_t led_id, uint16_t period);
void configure_flashes(int8_t led_id, uint16_t flashes);
void configure_fade(int8_t led_id, uint16_t fade);
/**
 * @brief Set LED to the normalized state specified for a given amount of @time with a given amount of @fade
 * 		  time = 0 would stay forever on that state and fade = 0 would be no fade.
 * @param norm_state normalized state of LED: 1 is ON, 0 is OFF.
 * @param led_id the id given by get_id()
 */
void set_value(int8_t led_id, uint8_t norm_state);
/**
 * @brief Flash LED with a given @period with a given amount of @fade and @flashes amount of time
 * 		  fade = 0 would be no fade and flashes = 0 would make the LED flash forever
 * @param led_id the id given by get_id()
 */
void flash(int8_t led_id);
/**
 * @brief Polls all LEDS to see if one of them has to change.
 */
void poll_leds();
/*******************************************************************************
 ******************************************************************************/

#endif // _LED_H_
