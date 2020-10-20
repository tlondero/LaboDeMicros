/*
 * Display.c
 *
 *  Created on: Sep 26, 2020
 *      Author: MAGT
 */
#include <stdbool.h>
//#include "../headers/led.h"
#include "../headers/led_old.h"
#include "../headers/Display.h"
#include "../headers/gpio.h"
#include "../headers/board.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******/

/******************
 7-SEGMENT DISPLAY PIN NAMING CONVENTION

 | |||||||A||||||| |
 | |			 | |
 |F|			 |B|
 | |			 | |
 | |||||||G||||||| |
 | |			 | |
 |E|			 |C|
 | |			 | |
 | |||||||D||||||| | |DP|

 ***********************/

#define PIN_A PORTNUM2PIN(PC, 1) //NEGRO
#define PIN_B PORTNUM2PIN(PC, 3) //BLANCO
#define PIN_C PORTNUM2PIN(PC, 16) //GRIS
#define PIN_D PORTNUM2PIN(PA, 1) //VIOLETA
#define PIN_E PORTNUM2PIN(PB, 18)  //AZUL
#define PIN_F PORTNUM2PIN(PD, 0) //VERDE
#define PIN_G PORTNUM2PIN(PD, 3)        //AMARILLO
#define PIN_DOT PORTNUM2PIN(PC, 9) //NARANJA

#define SEL_LINE_A PORTNUM2PIN(PC,0)			//MARRON (ESTA EN FRENTE)
#define SEL_LINE_B PORTNUM2PIN(PA,2)           //ROJO

#define SEVEN_SEGMENTS_PINS 8

#define REFRESH_RATE 3//ms
const uint8_t PINES[SEVEN_SEGMENTS_PINS] = { PIN_A, PIN_B, PIN_C, PIN_D, PIN_E,
PIN_F, PIN_G, PIN_DOT };

typedef struct {
	char name;
	uint8_t pin_mode[SEVEN_SEGMENTS_PINS]; // PIN_A, PIN_B, PIN_C, PIN_D, PIN_E, PIN_F, PIN_G, PIN_DOT
} character_t;

//PIN_A, PIN_B, PIN_C, PIN_D, PIN_E, PIN_F, PIN_G, PIN_DOT

const static character_t characters[] = {
		{'0', { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW} },
		{'1', { LOW, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW} },
		{'2', { HIGH, HIGH, LOW, HIGH, HIGH, LOW, HIGH, LOW} },
		{'3', { HIGH, HIGH, HIGH, HIGH, LOW, LOW, HIGH, LOW} },
		{'4', { LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW} },
		{'5', { HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH, LOW} },
		{'6', { HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH, LOW} },
		{'7', { HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW} },
		{'8', { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW} },
		{'9', { HIGH, HIGH, HIGH, HIGH, LOW, HIGH, HIGH, LOW} },
		{'A', {HIGH, HIGH, HIGH, LOW, HIGH, HIGH, HIGH, LOW} },
        {'B', {LOW, LOW, HIGH, HIGH, HIGH, HIGH, HIGH, LOW}},
        {'C', {HIGH, LOW, LOW, HIGH, HIGH, HIGH, LOW, LOW} },
        {'D', {LOW, HIGH, HIGH, HIGH, HIGH, LOW, HIGH, LOW} },
        {'E', {HIGH, LOW, LOW, HIGH, HIGH, HIGH, HIGH, LOW} },
        {'F', {HIGH, LOW, LOW, LOW, HIGH, HIGH, HIGH, LOW} },
        {'G', {HIGH, LOW, HIGH, HIGH, HIGH, HIGH, LOW, LOW} },
        {'H', {LOW, HIGH, HIGH, LOW, HIGH, HIGH, HIGH, LOW} },
        {'I', {LOW, LOW, LOW, LOW, HIGH, HIGH, LOW, LOW} },
        {'J', {LOW, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW} },
        {'K', {LOW, LOW, HIGH, LOW, HIGH, HIGH, HIGH, LOW} },
        {'L', {LOW, LOW, LOW, HIGH, HIGH, HIGH, LOW, LOW} },
        {'M', {HIGH, LOW, HIGH, LOW, HIGH, LOW, LOW, LOW} },
        {'N', {HIGH, HIGH, HIGH, LOW, HIGH, HIGH, LOW, LOW} },
        {'O', {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW} },
        {'P', {HIGH, HIGH, LOW, LOW, HIGH, HIGH, HIGH, LOW} },
        {'Q', {HIGH, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW} },
        {'R', {HIGH, HIGH, LOW, LOW, HIGH, HIGH, HIGH, LOW} },
        {'S', {HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH, LOW} },
        {'T', {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW} },
        {'U', {LOW, HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW} },
        {'V', {LOW, HIGH, HIGH, HIGH, LOW, HIGH, LOW, LOW} },
        {'W', {LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, LOW} },
        {'X', {LOW, HIGH, HIGH, LOW, HIGH, HIGH, HIGH, LOW} },
        {'Y', {LOW, HIGH, HIGH, HIGH, LOW, HIGH, HIGH, LOW} },
        {'Z', {HIGH, HIGH, LOW, HIGH, LOW, LOW, HIGH, LOW} },
		{'-', {LOW, LOW, LOW, LOW, LOW, LOW, HIGH, LOW} },
		{' ', {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW} },
		//      A    B    C   D    E     F    G   H
		{'^', {LOW, LOW, LOW, LOW, LOW, LOW, HIGH, LOW} },
		{'>', {LOW, LOW, HIGH, LOW, LOW, LOW, LOW, LOW} },
		{'_', {LOW, LOW, LOW, HIGH, LOW, LOW, LOW, LOW} },
		{'<', {LOW, LOW, LOW, LOW, HIGH, LOW, LOW, LOW} },
		{'.', {LOW, LOW, LOW, LOW, LOW, LOW, LOW, HIGH} }
};

typedef struct {
	bool enable;
	char ch;
} letter;

static letter displays[4] = { { false, '0' }, { false, '0' }, { false, '0' }, {
false, '0' } };

#define MAX_CHARACTERS sizeof(characters)/sizeof(character_t)

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
bool dispSetChar(char ch);
/*brief": Multiplexes the display
 * */
void multiplexDiplayCallback(void);
/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE DEFINITION
 ******************************************************************************/

void dispInit(void) {

	uint8_t i;
	led_init_driver(); // Initializes the leds driver.
	for (i = 0; i < SEVEN_SEGMENTS_PINS; i++) {
		//gpioMode(PINES[i], OUTPUT);
		seven_segment_id[i] = led_init_led(PINES[i], TURNS_ON_WITH_1);
		led_configure_brightness(seven_segment_id[i], 99);
	}

	gpioMode(SEL_LINE_A, OUTPUT);
	gpioWrite(SEL_LINE_A, LOW);
	gpioMode(SEL_LINE_B, OUTPUT);
	gpioWrite(SEL_LINE_B, LOW);
	timerInit();
	timer_id = timerGetId();
	timerStart(timer_id, TIMER_MS2TICKS(REFRESH_RATE), TIM_MODE_PERIODIC,
			multiplexDiplayCallback);
	//Cada 15 ms se multiplexea el display ~ cada uno aprox 50fps
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

bool dispSetChar(char ch) {
	int8_t i;
	int8_t j;

	bool ret = true;
	for (i = 0; i < MAX_CHARACTERS; i++) {
		if (characters[i].name == ch) {
			for (j = 0; j < SEVEN_SEGMENTS_PINS; j++) {
				led_set_state(seven_segment_id[j], characters[i].pin_mode[j]);
			}
			ret = true;
			break;
		}
	}
	if (i == MAX_CHARACTERS) {
		ret = false;
	}
	return ret;
}
void dispClearAll(void) {
	uint8_t i, j;
	for (i = 0; i < 4; i++) {
		if (dispSelect(i))
			for (j = 0; j < SEVEN_SEGMENTS_PINS; j++)
				led_set_state(PINES[j], LOW);
		displays[i].enable = false;
	}
}

void dispSendChar(char ch, uint8_t seven_seg_module) {
	if (seven_seg_module < 4) {
		displays[seven_seg_module].enable = true;
		displays[seven_seg_module].ch = ch;
	}
}

void multiplexDiplayCallback(void) {

	static uint8_t i = 0;
	if (displays[i % 4].enable) {
		dispSetChar(displays[(i) % 4].ch);
		dispSelect(i % 4);
	}
	i++;
}

void dispBrightness(uint8_t brightness){}
