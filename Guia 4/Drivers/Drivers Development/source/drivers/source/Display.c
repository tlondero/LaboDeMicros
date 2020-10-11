/*
 * Display.c
 *
 *  Created on: Sep 26, 2020
 *      Author: MAGT
 */
#include <stdbool.h>
#include "../headers/led.h"
#include "../headers/Display.h"
#include "../headers/gpio.h"
#include "../headers/board.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******/

/******************
7-SEGMENT DISPLAY PIN NAMING CONVENTION

| |||||||A||||||| |
| |				| |
|F|				|B|
| |				| |
| |||||||G||||||| |
| |				| |
|E|				|C|
| |				| |
| |||||||D||||||| |   |DP|

***********************/

#define PIN_A PORTNUM2PIN(PB, 18) //NEGRO
#define PIN_B PORTNUM2PIN(PC, 16) //BLANCO
#define PIN_C PORTNUM2PIN(PB, 19) //GRIS
#define PIN_D PORTNUM2PIN(PC, 17) //VIOLETA
#define PIN_E PORTNUM2PIN(PC, 1)  //AZUL
#define PIN_F PORTNUM2PIN(PB, 9) //VERDE
#define PIN_G PORTNUM2PIN(PC, 8)		//AMARILLO
#define PIN_DOT PORTNUM2PIN(PA, 1) //NARANJA
#define SEL_LINE_A PORTNUM2PIN(PC,0)			//MARRON (ESTA EN FRENTE)
#define SEL_LINE_B PORTNUM2PIN(PA,2)           //ROJO


#define SEVEN_SEGMENTS_PINS 8
const uint8_t PINES[SEVEN_SEGMENTS_PINS] = {PIN_A, PIN_B, PIN_C, PIN_D, PIN_E, PIN_F, PIN_G, PIN_DOT};

typedef struct
{
	char name;
	uint8_t pin_mode[SEVEN_SEGMENTS_PINS]; // PIN_A, PIN_B, PIN_C, PIN_D, PIN_E, PIN_F, PIN_G, PIN_DOT
} character_t;

//PIN_A, PIN_B, PIN_C, PIN_D, PIN_E, PIN_F, PIN_G, PIN_DOT

const static character_t characters[] = {

		{'0', {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW}},
		{'1', {LOW, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW}},
		{'2', {HIGH, HIGH, LOW, HIGH, HIGH, LOW, HIGH, LOW}},
		{'3', {HIGH, HIGH, HIGH, HIGH, LOW, LOW, HIGH, LOW}},
		{'4', {LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW}},
		{'5', {HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH, LOW}},
		{'6', {HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH, LOW}},
		{'7', {HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW}},
		{'8', {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW}},
		{'9', {HIGH, HIGH, HIGH, HIGH, LOW, HIGH, HIGH, LOW}}
};



#define MAX_CHARACTERS sizeof(characters)/sizeof(character_t)
/******************
7-SEGMENT DISPLAY PIN NAMING CONVENTION

| |||||||A||||||| |
| |				| |
|F|				|B|
| |				| |
| |||||||G||||||| |
| |				| |
|E|				|C|
| |				| |
| |||||||D||||||| |   |DP|

***********************/


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
static int8_t seven_segment_id[7] = {0};
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
/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE DEFINITION
 ******************************************************************************/

void dispInit(void)
{

	uint8_t i;
	led_init_driver(); // Initializes the leds driver.
	for (i = 0; i < SEVEN_SEGMENTS_PINS; i++)
		{
			gpioMode(PINES[i], OUTPUT);
			seven_segment_id[i] = led_init_led(PINES[i], TURNS_ON_WITH_1);
		}
	gpioMode(SEL_LINE_A, OUTPUT);
	gpioWrite(SEL_LINE_A, LOW);
	gpioMode(SEL_LINE_B, OUTPUT);
	gpioWrite(SEL_LINE_B, LOW);
	timerInit();
	timer_id = timerGetId();
}

bool dispSelect(int8_t disp)
{
	int8_t ret;
	switch (disp)
	{
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

bool dispSetChar(char ch)
{
	int8_t i;
	int8_t j;

	bool ret = true;
	for (i = 0; i < MAX_CHARACTERS; i++)
	{
		if (characters[i].name == ch)
		{
			for (j = 0; j < SEVEN_SEGMENTS_PINS; j++)
			{
				led_set_state(seven_segment_id[j], characters[i].pin_mode[j]);
			}
			ret = true;
			break;
		}
	}
	if (i == MAX_CHARACTERS)
	{
		ret = false;
	}
	return ret;
}
void dispClear(void)
{
	uint8_t i, j;
	for (i = 0; i < 4; i++)
	{
		if (dispSelect(i))
			for (j = 0; j < SEVEN_SEGMENTS_PINS; j++)
				led_set_state(PINES[j], LOW);
	}
}

bool dispSendChar(char ch, uint8_t seven_seg_module)
{
	bool ret = false;
	if (dispSelect(seven_seg_module))
	{
		ret = dispSetChar(ch);
	}
	return ret;
}
