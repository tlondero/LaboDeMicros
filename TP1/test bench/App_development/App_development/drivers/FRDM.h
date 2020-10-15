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
enum {
    GPIO_IRQ_MODE_DISABLE = 0,
    GPIO_IRQ_MODE_LOW_STATE = 8,
    GPIO_IRQ_MODE_HIGH_STATE = 12,
    GPIO_IRQ_MODE_RISING_EDGE = 9,
    GPIO_IRQ_MODE_FALLING_EDGE = 10,
    GPIO_IRQ_MODE_BOTH_EDGES = 11,

    GPIO_IRQ_CANT_MODES
};
#endif // _LED_H_
#define BUTTON_SW2 0
#define BUTTON_SW3 1
bool FRDMButtonIRQ(uint8_t button, uint8_t IRQ_mode, pinIrqFun_t fcallback);