/***************************************************************************//**
 @file     PV.c
 @brief    
 @author   MAGT
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "../headers/PV.h"
#include "../headers/board.h"
#include "../headers/gpio.h"
#include "../headers/timer.h"
#include "../../app/headers/fsm.h"
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define	PV_BUTTON		PORTNUM2PIN(PB,2)			//VER PIN PORQUE NO TENGO NI PUTA IDEA!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define PIN_C2_EN		PORTNUM2PIN(PC,2)			//VER PIN PORQUE NO TENGO NI PUTA IDEA!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define PIN_C7_EN		PORTNUM2PIN(PC,7)			//VER PIN PORQUE NO TENGO NI PUTA IDEA!!!!!!!!!!!!!!!!!!!!!!!!!!!

#define LED_LINE_A		PORTNUM2PIN(PC,11)			//MARRON (ESTA EN FRENTE)
#define LED_LINE_B 		PORTNUM2PIN(PC,10)           //ROJO

#define LED_IN_PV		3
#define DEC_IN_PV		2
#define EXTERN_LEDS		3

#define SEV_SEG			4

const uint8_t ST_PIN[DEC_IN_PV] = { LED_LINE_A, LED_LINE_B };

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH LOCAL SCOPE
 ******************************************************************************/

//Button
static PVButton_t button;
static PVEncoder_t idEncoder;
static bool button_is_pressed  = false;

static bool isEvent = false;
static PVEv_t event = NO_PV_EV;
static uint8_t listEv[NO_PV_EV] = { 0 };

//Extern leds
static int8_t idLed[EXTERN_LEDS] = { 0 };

//Display
static int8_t dispBright;
static int8_t deltaBright;
static PVDirection_t dir;

static char message[MAX_MESS_LEN];
static uint8_t length;
static int8_t countMess;
static uint32_t mrqtime = 1000;

//Timers
static tim_id_t timer_open_st;
static tim_id_t timer_id_mrq;
static tim_id_t timer_id_button; //Para evitar rebotes
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

bool PVLedSelect(PVLed_t led);
void open_animation_Callback(void);
void multiplexLedCallback(void);
/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void dispShowText(void) {
	uint8_t i = 0;
	for (i = 0; i < SEV_SEG; i++) {
		dispSendChar(message[(i + countMess) % length], i);
	}

	switch (dir) {
	case (PV_LEFT):
		countMess++;
		if ((countMess + SEV_SEG) > length) {
			countMess = 0;
		}
		break;
	case (PV_RIGHT):
		countMess--;
		if ((countMess - SEV_SEG) < 0) {
			countMess = length - SEV_SEG;
		}
		break;
	case (PV_NODIR):
		break;
	default:
		break;
	}
}

void open_animation_Callback(void) {
	static uint8_t i = 0, j;
	static char open_animation[4] = { '^', '>', '_', '<' };
	//static char open_animation[7] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g' };
	if (i == OPEN_TRIGGER_TIME / 500) {
		timerStop(timer_open_st);
	} else {
		for (j = 0; j < 4; j++)
			dispSendChar(open_animation[i % 4], j);
	}
	i++;
}

uint8_t checkMessageLength(char *mes) {
	uint8_t i = 0;
	while (mes[i] != '\0') {
		i++;
	}
	return i + 1;
}

void PVButtonPressedCallback(void){
	if(timerExpired(timer_id_button))
	{
		button_is_pressed = true;
		timerReset(timer_id_button);
	}else{
		button_is_pressed = false;
	}
}

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

bool PVInit(void) {

	//Button init
	timer_id_button = timerGetId();
	timerStart(timer_id_button, 200, TIM_MODE_SINGLESHOT, NULL);

	button = ButtonInit(PV_BUTTON, INPUT_PULLUP, LOW_WHEN_PRESSED);
	ButtonSetIRQ(button, GPIO_IRQ_MODE_FALLING_EDGE, PVButtonPressedCallback);
	//Encoder init
	idEncoder = EncoderRegister(PIN_C2_EN, PIN_C7_EN);

	//Display init
	dispInit();
	dispBright = 20;
	deltaBright = 20;
	dir = PV_LEFT;

	led_init_driver();
	message[0] = '\0';
	length = 0;
	countMess = 0;

	//LED STATUS INIT
	bool okLed = true;

	for (uint8_t i = 0; i < DEC_IN_PV; i++) {
		gpioMode(ST_PIN[i], OUTPUT);
		gpioWrite(ST_PIN[i], LOW);
	}

	//PORTNUM2PIN(p,n)
	idLed[0] = led_init_led(PORTNUM2PIN(PB, 22), TURNS_ON_WITH_0);
	idLed[2] = led_init_led(PORTNUM2PIN(PB, 21), TURNS_ON_WITH_0);
	idLed[1] = led_init_led(PORTNUM2PIN(PE, 26), TURNS_ON_WITH_0);

	/*
	 idLed[0] = led_init_led(PB, 22, TURNS_ON_WITH_0);
	 idLed[2] = led_init_led(PB, 21, TURNS_ON_WITH_0);
	 idLed[1] = led_init_led(PE, 26, TURNS_ON_WITH_0);
	 */

	if ((idLed[0] == -1) || (idLed[1] == -1) || (idLed[2] == -1)) {
		okLed = false;
	} else {

		//Led default config
		uint8_t bright = 10;
		uint32_t fade = 100;			//ms
		uint32_t dt = 50;				//%
		uint8_t flashes = 0;
		uint32_t period = 1000;		//ms
		uint32_t time = 0;		//ms

		uint8_t i;

		for (i = 0; i < EXTERN_LEDS; i++) {
			led_configure_brightness(idLed[i], bright);
			led_configure_time(idLed[i], time);
			led_configure_fade(idLed[i], fade);
			led_configure_dt(idLed[i], dt);
			led_configure_flashes(idLed[i], flashes);
			led_configure_period(idLed[i], period);

			led_set_state(idLed[i], LOW);
		}
	}

	//timer init
	timerInit();
	timer_id_mrq = timerGetId();
	timer_open_st = timerGetId();

	timerStart(timer_open_st, TIMER_MS2TICKS((5)), TIM_MODE_PERIODIC, open_animation_Callback);
	//timerStart(timer_open_st, TIMER_MS2TICKS((500)), TIM_MODE_PERIODIC, open_animation_Callback);
	timerStop(timer_open_st);

	timerStart(timer_id_mrq, TIMER_MS2TICKS(1000), TIM_MODE_PERIODIC,
			dispShowText);
	timerStop(timer_id_mrq);
	return okLed;
}

void PVSuscribeEvent(PVEv_t ev, bool state) {
	listEv[ev] = state;
}

bool PVCheckEvent(void) {
	if ((button_is_pressed == true) || (EncoderEventAVB(idEncoder) == EVENT_AVB))
		isEvent = true;
	else
		isEvent = false;

	return isEvent;
}

PVEv_t PVGetEv(void) {
	if (isEvent == true) {

		if (EncoderEventAVB(idEncoder)) {
			event_t ev = EncoderPopEvent(idEncoder);
			if ((ev == RIGHT_TURN) && (listEv[ENC_RIGHT])) {
				event = ENC_RIGHT;
			} else if ((ev == LEFT_TURN) && (listEv[ENC_LEFT])) {
				event = ENC_LEFT;
			} else {
				event = NO_PV_EV;
			}
		} else if (button_is_pressed == true) {
			event = BTN_PRESS;
			button_is_pressed = false;
		}
	} else {
		event = NO_PV_EV;
	}

	return event;
}

bool PVButtonIRQ(PVIRQMode_t IRQ_mode, pinIrqFun_t fcallback) {
	uint8_t IRQ = PV_CANT_MODES;
	bool correct_mode = true;
	switch (IRQ_mode) {
	case (PV_DISABLE):
		IRQ = GPIO_IRQ_MODE_DISABLE;
		break;
	case (PV_LSTATE):
		IRQ = GPIO_IRQ_MODE_LOW_STATE;
		break;
	case (PV_HSTATE):
		IRQ = GPIO_IRQ_MODE_HIGH_STATE;
		break;
	case (PV_REDGE):
		IRQ = GPIO_IRQ_MODE_RISING_EDGE;
		break;
	case (PV_FEDGE):
		IRQ = GPIO_IRQ_MODE_FALLING_EDGE;
		break;
	case (PV_BEDGES):
		IRQ = GPIO_IRQ_MODE_BOTH_EDGES;
		break;
	default:
		correct_mode = false;
		break;
	}

	if (correct_mode) {
		ButtonSetIRQ(button, IRQ, fcallback);
		return true;
	} else {
		return false;
	}
}

void PVDisplayClear(void) {
	uint8_t i;
	for(i=0; i < MAX_MESS_LEN; i++){
		message[i] = '\0';
	}
	length = 0;
	countMess = 0;

	timerReset(timer_id_mrq);
	timerStop(timer_id_mrq);

	dispClearAll();
}

bool PVDisplaySetBright(uint8_t br) {
	bool valid = true;
	if ((br >= 1) && (br <= 100)) {
		dispBright = br;
		dispBrightness(dispBright);
	} else if (br == 0) {
		dispClearAll();
	} else {
		valid = false;
	}
	return valid;
}

bool PVIncreaseBrightness(void) {
	bool topValue = false;
	dispBright += deltaBright;
	if (dispBright >= 100) {
		dispBright = 100;
		topValue = true;
	}
	dispBrightness(dispBright);
	return topValue;
}

bool PVDecreaseBrightness(void) {
	bool bottomValue = false;
	dispBright -= deltaBright;
	if (dispBright <= 0) {
		dispBright = 0;
		bottomValue = true;
		dispClearAll();
	} else {
		dispBrightness(dispBright);
	}
	return bottomValue;
}

int8_t PVGetBrightness(void) {
	return dispBright;
}

int8_t PVGetDeltaBright(void) {
	return deltaBright;
}

bool PVSetDeltaBright(uint8_t br) {
	bool valid = false;
	if ((br >= 0) && (br <= 100)) {
		deltaBright = br;
		valid = true;
	}
	return valid;
}

bool PVDisplaySendChar(char ch, uint8_t seven_seg_module) {

	if (message[0] != '\0') {
		timerReset(timer_id_mrq);
		timerStop(timer_id_mrq);
		message[0] = '\0';
		length = 0;
		countMess = 0;
	}

	bool valid = false;

	if(dispBright == 0){
		dispClearAll();
	} else if (seven_seg_module < SEV_SEG) {
		dispSendChar(ch, seven_seg_module);
		valid = true;
	}

	return valid;
}

bool PVMarquesina(char *mess, uint32_t time_per_char) {
	bool valid = true;
	uint8_t l = checkMessageLength(mess);
	if (l < MAX_MESS_LEN) {

		PVDisplaySetTime(time_per_char);
		//Pongo todo en mayusculas
		uint8_t i;
		char tempString1[l];
		char tempString2[l + 2 * SEV_SEG];

		for (i = 0; i < l; i++) {
			char aux = mess[i];
			if ((aux >= 97) && (aux <= 122)) {
				aux -= 32;
			}
			tempString1[i] = aux;
		}

		//Agrego 4 ' ' al principio y 4 al final
		for (i = 0; i < l + 7; i++) {
			if (i < SEV_SEG) {
				tempString2[i] = 32;
			} else if (i < 3 + l) {
				tempString2[i] = tempString1[i - SEV_SEG];
			} else {
				tempString2[i] = 32;
			}
		}
		tempString2[l + 7] = '\0';

		for (i = 0; i < l + 2 * SEV_SEG; i++) {
			message[i] = tempString2[i];
		}

		length = l + 2 * SEV_SEG;
		countMess = 0; //4?
	} else {
		message[0] = '\0';
		length = 0;
		countMess = 0;
		valid = false;
	}

	timerReset(timer_id_mrq);

	PVDispMessOn();

	return valid;
}

bool PVDispManualShift(PVDirection_t direction, uint8_t cant) {

	bool valid = true;

	if ((countMess + cant < length) && (countMess - cant > 0)) {
		switch (direction) {
		case (PV_LEFT):
			countMess += cant;
			break;
		case (PV_RIGHT):
			countMess -= cant;
			break;
		default:
			valid = false;
			break;
		}
	} else {
		valid = false;
	}
	return valid;
}

bool PVDispMessOn(void) {
	bool valid = false;
	if (message[0] != '\0') {
		timerResume(timer_id_mrq);
		valid = true;
	}

	return valid;
}

bool PVDisplaySetShift(PVDirection_t direction) {

	bool valid = true;

	switch (direction) {
	case (PV_RIGHT):
		dir = PV_RIGHT;
		break;
	case (PV_LEFT):
		dir = PV_LEFT;
		break;
	case (PV_NODIR):
		dir = PV_NODIR;
		break;
	default:
		dir = PV_NODIR;
		valid = false;
		break;
	}
	return valid;
}

bool PVDisplaySetTime(uint32_t time) {

	bool valid = false;

	if (time != 0) {
		mrqtime = time;
		timerStart(timer_id_mrq, TIMER_MS2TICKS(time), TIM_MODE_PERIODIC,
				dispShowText);
		valid = true;
	}

	return valid;
}

bool PVAnimation(animation_t animation, bool activate) {
	bool valid = true;
	if (activate) {
		switch (animation) {
		case IDDLE_ANIMATION:
			break;
		case ASK_PIN_ANIMATION:
			break;
		case ACCESS_ANIMATION:
			break;
		case OPEN_ANIMATION:
			timerReset(timer_open_st);
			timerResume(timer_open_st);
			break;
		case USERS_ANIMATION:
			break;
		case BRIGHTNESS_ANIMATION:
			break;
		case INVALID_ID_ANIMATION:
			break;
		default:
			valid = false;
			break;
		}
	} else {
		PVDisplayClear();
	}
	return valid;
}

bool PVLedSetBright(PVLed_t led, uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		double newVal = value;
		valid = true;
		switch (led) {
		case (PV_LED_1):
			led_configure_brightness(idLed[0], newVal);
			break;
		case (PV_LED_2):
			led_configure_brightness(idLed[1], newVal);
			break;
		case (PV_LED_3):
			led_configure_brightness(idLed[2], newVal);
			break;
		case (PV_LED_ALL):
			for (uint8_t i = 0; i < EXTERN_LEDS; i++) {
				led_configure_brightness(idLed[i], newVal);
			}
			break;
		default:
			valid = false;
		}
	}
	return valid;
}

bool PVLedSetFade(PVLed_t led, uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		valid = true;
		switch (led) {
		case (PV_LED_1):
			led_configure_fade(idLed[0], value);
			break;
		case (PV_LED_2):
			led_configure_fade(idLed[1], value);
			break;
		case (PV_LED_3):
			led_configure_fade(idLed[2], value);
			break;
		case (PV_LED_ALL):
			for (uint8_t i = 0; i < EXTERN_LEDS; i++) {
				led_configure_fade(idLed[i], value);
			}
			break;
		default:
			valid = false;
		}
	}
	return valid;
}

bool PVLedSetDt(PVLed_t led, uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		uint8_t realdt = 100 - value;
		valid = true;
		switch (led) {
		case (PV_LED_1):
			led_configure_dt(idLed[0], realdt);
			break;
		case (PV_LED_2):
			led_configure_dt(idLed[1], realdt);
			break;
		case (PV_LED_3):
			led_configure_dt(idLed[2], realdt);
			break;
		case (PV_LED_ALL):
			for (uint8_t i = 0; i < EXTERN_LEDS; i++) {
				led_configure_fade(idLed[i], value);
			}
			break;
		default:
			valid = false;
		}
	}
	return valid;
}

bool PVLedSetFlash(PVLed_t led, uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		valid = true;
		switch (led) {
		case (PV_LED_1):
			led_configure_flashes(idLed[0], value);
			break;
		case (PV_LED_2):
			led_configure_flashes(idLed[1], value);
			break;
		case (PV_LED_3):
			led_configure_flashes(idLed[2], value);
			break;
		case (PV_LED_ALL):
			for (uint8_t i = 0; i < EXTERN_LEDS; i++) {
				led_configure_fade(idLed[i], value);
			}
			break;
		default:
			valid = false;
		}
	}
	return valid;
}

bool PVLedSetPeriod(PVLed_t led, uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		valid = true;
		switch (led) {
		case (PV_LED_1):
			led_configure_period(idLed[0], value);
			break;
		case (PV_LED_2):
			led_configure_period(idLed[1], value);
			break;
		case (PV_LED_3):
			led_configure_period(idLed[2], value);
			break;
		case (PV_LED_ALL):
			for (uint8_t i = 0; i < EXTERN_LEDS; i++) {
				led_configure_fade(idLed[i], value);
			}
			break;
		default:
			valid = false;
		}
	}
	return valid;
}

bool PVLedSetTime(PVLed_t led, uint8_t value) {
	bool valid = true;
	switch (led) {
	case (PV_LED_1):
		led_configure_time(idLed[0], value);
		break;
	case (PV_LED_2):
		led_configure_time(idLed[1], value);
		break;
	case (PV_LED_3):
		led_configure_time(idLed[2], value);
		break;
	case (PV_LED_ALL):
		for (uint8_t i = 0; i < EXTERN_LEDS; i++) {
			led_configure_fade(idLed[i], value);
		}
		break;
	default:
		valid = false;
	}
	return valid;
}

bool PVLedFlash(PVLed_t led, uint8_t color) {

	bool valid = false;
	valid = true;
	switch (led) {
	case (PV_LED_1):
		led_flash(idLed[0]);
		break;
	case (PV_LED_2):
		led_flash(idLed[1]);
		break;
	case (PV_LED_3):
		led_flash(idLed[2]);
		break;
	case (PV_LED_ALL):
		for (uint8_t i = 0; i < EXTERN_LEDS; i++) {
			led_flash(idLed[i]);
		}
		break;
	default:
		valid = false;
		break;
	}
	return valid;
}

bool PVLedOn(PVLed_t led) {

	bool valid = false;
	switch (led) {
	case (PV_LED_1):
		led_set_state(idLed[0], HIGH);
		break;
	case (PV_LED_2):
		led_set_state(idLed[1], HIGH);
		break;
	case (PV_LED_3):
		led_set_state(idLed[2], HIGH);
		break;
	case (PV_LED_ALL):
		for (uint8_t i = 0; i < EXTERN_LEDS; i++) {
			led_set_state(idLed[i], HIGH);
		}
		break;
	default:
		valid = false;
		break;
	}
	return valid;
}

void PVLedOff(PVLed_t led) {
	switch (led) {
	case (PV_LED_1):
		led_set_state(idLed[0], LOW);
		break;
	case (PV_LED_2):
		led_set_state(idLed[1], LOW);
		break;
	case (PV_LED_3):
		led_set_state(idLed[2], LOW);
		break;
	case (PV_LED_ALL):
		for (uint8_t i = 0; i < EXTERN_LEDS; i++) {
			led_set_state(idLed[i], LOW);
		}
		break;
	default:
		break;
	}
}

void PVLedPoll(void) {
	led_poll();
}

void PVStatusLedSelect(PVStatus_t led, bool state) {
	if (state) {
		switch (led) {
		case (ON_ST_1):
			gpioWrite(LED_LINE_A, !HIGH);
			gpioWrite(LED_LINE_B, !LOW);
			break;
		case (ON_ST_2):
			gpioWrite(LED_LINE_A, !LOW);
			gpioWrite(LED_LINE_B, !HIGH);
			break;
		case (ON_ST_3):
			gpioWrite(LED_LINE_A, !LOW);
			gpioWrite(LED_LINE_B, !LOW);
			break;
		case (ON_ST_OFF):
			gpioWrite(LED_LINE_A, LOW);
			gpioWrite(LED_LINE_B, LOW);
			break;
		default:
			gpioWrite(LED_LINE_A, LOW);
			gpioWrite(LED_LINE_B, LOW);
			break;
		}
	} else {
		gpioWrite(LED_LINE_A, LOW);
		gpioWrite(LED_LINE_B, LOW);
	}
}
