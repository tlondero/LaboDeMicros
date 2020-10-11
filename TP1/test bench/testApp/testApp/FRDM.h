#pragma once
#include <stdint.h>
#include <stdbool.h>
typedef void (*pinIrqFun_t)(void);
typedef uint8_t pin_t;
enum {
	NO_EV,//No event was detected by the button
	PRESS,//Button was pressed
	RELEASE,//button was released
	LKP,// Long Key Press
	SKP, //Short key press  //Finisher of the ev queue
	EOQ
};

typedef uint8_t ButtonEvent;//instant events such as PRESS and RELEASE


void FRDMInit(void);
int8_t ButtonInit(pin_t pin, uint8_t mode);
void ButtonSetIRQ(int8_t id, uint8_t IRQ_mode, pinIrqFun_t fcallback);
const ButtonEvent* ButtonGetEvent(int8_t id);



#ifndef _LED_H_
#define _LED_H_

 /*******************************************************************************
  * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
  ******************************************************************************/
int8_t led_init_led(pin_t pin, uint8_t pin_mode);
void led_destroy_led(int8_t led_id);
void led_configure(void);
void led_set_state(int8_t led_id, uint8_t norm_state);
void led_flash(int8_t led_id);
void led_poll(void);
/*******************************************************************************
 ******************************************************************************/

#endif // _LED_H_
