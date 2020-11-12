/*
 * Display.c
 *
 *  Created on: Sep 26, 2020
 *      Author: MAGT
 */
#include "header/led.h"
#include "header/Display.h"
#include "header/gpio.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******/

#define PIN_A 0
#define PIN_B 0
#define PIN_C 0
#define PIN_D 0
#define PIN_E 0
#define PIN_F 0
#define PIN_G 0
#define PIN_DOT 0
#define SEL_LINE_A 0
#define SEL_LINE_B 0

const uint8_t PINES[8] = { PIN_A, PIN_B, PIN_C, PIN_D, PIN_E, PIN_F, PIN_G, PIN_DOT };

//////////////////////////// ASCII CHARACTERS
enum {
	CH_0 = 48, CH_1, CH_2, CH_3, CH_4, CH_5, CH_6, CH_7, CH_8, CH_9
};

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
static int8_t seven_segment_id[7] = { 0 };
static tim_id_t timer_id;
/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE DECLARATION
 ******************************************************************************/
/**
 * @brief dispSelect: Lets you select the 7 segment to use.
 * @param disp: 0~3 for the 7 segmetns.
 * @return bool: true if the assignment was successful, false otherwise.
 *
 */
bool dispSelect(int8_t disp);
/**
 * @brief dispSetChar: Assigns the character sent by the user to the 7 segments pins.
 * @param ch:Character to be written, check the define section for available characters.
 * @return bool, returns false if the character was invalid.
 */
bool dispSetChar(int8_t ch);
/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE DEFINITION
 ******************************************************************************/

void dispInit(void) {
	uint8_t i;
	led_init_driver(); // Initializes the leds driver.
	for (i = 0; i < 8; i++)
		seven_segment_id[i] = led_init_led(PINES[i], TURNS_ON_WITH_1);
	//////////////////////////////////////////////////////////
	///////////PREGUNTAR TINCHO TURN ON MODE !!!//////////////
	//////////////////////////////////////////////////////////
	gpioMode(SEL_LINE_A, OUTPUT);
	gpioMode(SEL_LINE_B, OUTPUT);
	timerInit();
	timer_id = timerGetId();

}

bool dispSelect(int8_t disp) {
	int8_t ret;
	switch (disp) {
	case 0:
		gpioWrite(SEL_LINE_A, LOW);
		gpioWrite(SEL_LINE_B, LOW);
		ret = true;
		break;
	case 1:
		gpioWrite(SEL_LINE_A, HIGH);
		gpioWrite(SEL_LINE_B, LOW);
		ret = true;
		break;
	case 2:
		gpioWrite(SEL_LINE_A, LOW);
		gpioWrite(SEL_LINE_B, HIGH);
		ret = true;
		break;
	case 3:
		gpioWrite(SEL_LINE_A, HIGH);
		gpioWrite(SEL_LINE_B, HIGH);
		ret = true;
		break;
	default:
		ret = false;
	}
	return ret;
}

bool dispSetChar(int8_t ch) {
	int8_t i;
	bool ret=true;
	for (i = 0; i < 8; i++)
		led_set_state(PINES[i], LOW);
	switch (ch) {
	case CH_1:
		led_set_state(PIN_B, HIGH);
		led_set_state(PIN_C, HIGH);
		break;
	case CH_2:
		led_set_state(PIN_A, HIGH);
		led_set_state(PIN_B, HIGH);
		led_set_state(PIN_G, HIGH);
		led_set_state(PIN_E, HIGH);
		led_set_state(PIN_D, HIGH);
		break;
	case CH_3:
		led_set_state(PIN_A, HIGH);
		led_set_state(PIN_B, HIGH);
		led_set_state(PIN_G, HIGH);
		led_set_state(PIN_C, HIGH);
		led_set_state(PIN_D, HIGH);
		break;
	case CH_4:
		led_set_state(PIN_F, HIGH);
		led_set_state(PIN_B, HIGH);
		led_set_state(PIN_C, HIGH);
		led_set_state(PIN_G, HIGH);
		break;
	case CH_5:
		led_set_state(PIN_A, HIGH);
		led_set_state(PIN_F, HIGH);
		led_set_state(PIN_G, HIGH);
		led_set_state(PIN_C, HIGH);
		led_set_state(PIN_D, HIGH);
		break;
	case CH_6:
		led_set_state(PIN_F, HIGH);
		led_set_state(PIN_A, HIGH);
		led_set_state(PIN_C, HIGH);
		led_set_state(PIN_E, HIGH);
		led_set_state(PIN_D, HIGH);
		led_set_state(PIN_G, HIGH);
		break;
	case CH_7:
		led_set_state(PIN_A, HIGH);
		led_set_state(PIN_B, HIGH);
		led_set_state(PIN_C, HIGH);
		break;
	case CH_8:
		led_set_state(PIN_A, HIGH);
		led_set_state(PIN_B, HIGH);
		led_set_state(PIN_C, HIGH);
		led_set_state(PIN_D, HIGH);
		led_set_state(PIN_E, HIGH);
		led_set_state(PIN_F, HIGH);
		led_set_state(PIN_G, HIGH);
		break;
	case CH_9:
		led_set_state(PIN_A, HIGH);
		led_set_state(PIN_B, HIGH);
		led_set_state(PIN_C, HIGH);
		led_set_state(PIN_D, HIGH);
		led_set_state(PIN_F, HIGH);
		led_set_state(PIN_G, HIGH);
		break;
	default:
		ret=false;
	}
	return ret;
}
void dispClear(void) {
	uint8_t i, j;
	for (i = 0; i < 4; i++) {
		if (dispSelect(i))
			for (j = 0; j < 8; j++)
				led_set_state(PINES[j], LOW);
	}
}


bool dispSendChar(char ch, uint8_t seven_seg_module){
	bool ret=false;
	if(dispSelect(seven_seg_module)){
		ret = dispSetChar(ch);
	}
	return ret;
}
