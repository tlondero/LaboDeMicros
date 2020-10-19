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

#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define	PV_BUTTON		PORTNUM2PIN(PC,0)			//VER PIN PORQUE NO TENGO NI PUTA IDEA!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define PIN_C2_EN		PORTNUM2PIN(PC,2)			//VER PIN PORQUE NO TENGO NI PUTA IDEA!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define PIN_C7_EN		PORTNUM2PIN(PC,7)			//VER PIN PORQUE NO TENGO NI PUTA IDEA!!!!!!!!!!!!!!!!!!!!!!!!!!!

#define LED_LINE_A		PORTNUM2PIN(PC,0)			//MARRON (ESTA EN FRENTE)
#define LED_LINE_B 		PORTNUM2PIN(PA,2)           //ROJO

#define LED_IN_PV		3
#define DEC_IN_PV		2

const uint8_t ST_PIN[LED_IN_PV] = { LED_LINE_A, LED_LINE_B };

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

static bool isEvent = false;
static PVEv_t event = NO_PV_EV;
static uint8_t listEv[NO_PV_EV] = { 0 };

//Extern leds
static int8_t idLed1;
static int8_t idLed2;
static int8_t idLed3;

//Intern leds
static bool leds_st[LED_IN_PV] = { 0 };
static uint8_t count;

//Display
static int8_t dispBright;

static char *message;
static uint8_t length;
static uint8_t countMess;
static tim_id_t timer_id_mrq;

//Timer
static tim_id_t timer_id_st;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

bool PVLedSelect(PVLed_t led);

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

void multiplexLedCallback(void) {
	if (leds_st[0] || leds_st[1] || leds_st[2]) {
		if ((count == 0) && (leds_st[0])) {
			gpioWrite(LED_LINE_A, HIGH);
			gpioWrite(LED_LINE_B, LOW);
		} else if ((count == 1) && (leds_st[1])) {
			gpioWrite(LED_LINE_A, LOW);
			gpioWrite(LED_LINE_B, HIGH);
		} else if ((count == 2) && (leds_st[2])) {
			gpioWrite(LED_LINE_A, LOW);
			gpioWrite(LED_LINE_B, LOW);
		}
		if (++count == 4) {
			count = 0;
		}
	} else {
		gpioWrite(LED_LINE_A, HIGH);
		gpioWrite(LED_LINE_B, HIGH);
	}
}

uint8_t checkMessageLength(char *mes) {
	uint8_t i = 0;
	while (mes[i] != '\0') {
		i++;
	}
	return i + 1;
}

char* setMayus(char *mes, uint8_t len) {
	uint8_t i = 0;
	char mayusSt[len];
	for (i = 0; i < len; i++) {
		char aux = mes[i];
		if ((aux >= 97) && (aux <= 122)) {
			aux -= 32;
		}
		mayusSt[i] = aux;
	}
	return &mayusSt;
}

char* addSpaces(char *mes, uint8_t len) {

	char newMes[len + 8];      //2x4 spaces + \0
	uint8_t i = 0;
	while (i < len + 7) {
		if (i < 4) {
			newMes[i] = 32;
		} else if (i < 3 + len) {
			newMes[i] = mes[i - 4];
		} else {
			newMes[i] = 32;
		}
		i++;
	}
	newMes[len + 8] = '\0';

	return &newMes;
}

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

bool PVInit(void) {

	PVDispSetMess("suculento");
	char * xd = addSpaces(message, length);

	//Button init
	button = ButtonInit(PV_BUTTON, INPUT_PULLUP);

	//Encoder init
	idEncoder = EncoderRegister(PIN_C2_EN, PIN_C7_EN);

	//Display init
	dispInit();
	dispBright = 20;

	led_init_driver();
	message = NULL;
	length = 0;
	countMess = 0;

	//LED STATUS INIT
	bool okLed = true;

	count = 0;
	for (uint8_t i = 0; i < LED_IN_PV; i++) {
		gpioMode(ST_PIN[i], OUTPUT);
		gpioWrite(ST_PIN[i], HIGH);
	}

	idLed1 = led_init_led(PB, 22, TURNS_ON_WITH_0);
	idLed3 = led_init_led(PB, 21, TURNS_ON_WITH_0);
	idLed2 = led_init_led(PE, 26, TURNS_ON_WITH_0);

	if ((idLed1 == -1) || (idLed2 == -1) || (idLed3 == -1)) {
		okLed = false;
	} else {

		//Led default config
		uint32_t bright = 100;
		uint32_t fade = 100;			//ms
		uint32_t dt = 50;				//%
		uint8_t flashes = 0;
		uint32_t period = 1000;		//ms
		uint32_t time = 0;		//ms

		led_configure_brightness(idLed1, bright);
		led_configure_time(idLed1, time);
		led_configure_fade(idLed1, fade);
		led_configure_dt(idLed1, dt);
		led_configure_flashes(idLed1, flashes);
		led_configure_period(idLed1, period);

		led_configure_brightness(idLed3, bright);
		led_configure_time(idLed3, time);
		led_configure_fade(idLed3, fade);
		led_configure_dt(idLed3, dt);
		led_configure_flashes(idLed3, flashes);
		led_configure_period(idLed3, period);

		led_configure_brightness(idLed2, bright);
		led_configure_time(idLed2, time);
		led_configure_fade(idLed2, fade);
		led_configure_dt(idLed2, dt);
		led_configure_flashes(idLed2, flashes);
		led_configure_period(idLed2, period);

		led_set_state(idLed1, LED_OFF);
		led_set_state(idLed3, LED_OFF);
		led_set_state(idLed2, LED_OFF);
	}

	//timer init
	timerInit();
	timer_id_st = timerGetId();
	timer_id_mrq = timerGetId();
	timerStart(timer_id_st, TIMER_MS2TICKS(100), TIM_MODE_PERIODIC,
			multiplexLedCallback);

	return okLed;
}

void PVSuscribeEvent(PVEv_t ev, bool state) {
	listEv[ev] = state;
}

bool PVCheckEvent(void) {

	if (EncoderEventAVB(idEncoder)) {
		event_t ev = EncoderPopEvent(idEncoder);
		if ((ev == RIGHT_TURN) && (listEv[ENC_RIGHT])) {
			event = ENC_RIGHT;
		} else if ((ev == LEFT_TURN) && (listEv[ENC_LEFT])) {
			event = ENC_LEFT;
		} else {
			event = NO_PV_EV;
		}
	} else {
		uint8_t btnev = *ButtonGetEvent(button);
		if ((btnev != NO_EV) && (btnev != EOQ)) {
			if ((btnev == PRESS) && (listEv[BTN_PRESS])) {
				event = BTN_PRESS;
			} else if ((btnev == RELEASE) && (listEv[BTN_RELEASE])) {
				event = BTN_RELEASE;
			} else if ((btnev == LKP) && (listEv[BTN_LKP])) {
				event = BTN_LKP;
			} else if ((btnev == SKP) && (listEv[BTN_SKP])) {
				event = BTN_SKP;
			} else if ((btnev == NO_EV) && (listEv[NO_PV_EV])) {
				event = NO_PV_EV;
			} else {
				event = NO_PV_EV;
			}
		}
	}
	if (event != NO_PV_EV) {
		isEvent = true;
	} else {
		isEvent = false;
	}
	return isEvent;
}

PVEv_t PVGetEv(void) {
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
	dispClearAll();
}

bool PVDisplaySetBright(uint8_t br) {
	bool valid = true;
	if ((br >= 1) && (br <= 100)) {
		dispBright = br;
		PVDisplaySetBright(dispBright);
	} else if (br == 0) {
		dispClearAll();
	} else {
		valid = false;
	}
	return valid;
}

bool PVIncreaseBrightness(void) {
	bool topValue = false;
	dispBright += 20;
	if (dispBright >= 100) {
		dispBright = 100;
		topValue = true;
	}
	dispBrightness(dispBright);
	return topValue;
}

bool PVDecreaseBrightness(void) {
	bool bottomValue = false;
	dispBright -= 20;
	if (dispBright <= 0) {
		dispBright = 0;
		bottomValue = true;
		dispClearAll();
	} else {
		dispBrightness(dispBright);
	}
	return bottomValue;
}

bool PVDisplaySendChar(char ch, uint8_t seven_seg_module) {

	bool valid = false;

	if (seven_seg_module < 4) {
		dispSendChar(ch, seven_seg_module);
		valid = true;
	}

	return valid;
}

bool PVDispSetMess(char *mess) {
	bool valid = true;
	uint8_t l = checkMessageLength(mess);
	if (l < MAX_MESS_LEN) {
		message = setMayus(mess, l);
		length = l;
		countMess = 0;
	} else {
		message = NULL;
		length = 0;
		countMess = 0;
		valid = false;
	}

	return valid;
}

bool PVDisplayShift(PVDirection_t direction) {

	bool valid = true;

	if (direction == PV_RIGHT) {
		dispShift(RIGHT);
	} else if (direction == PV_LEFT) {
		dispShift(LEFT);
	} else {
		valid = false;
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
	}
	return valid;
}

bool PVLedSetBright(PVLed_t led, uint8_t value) {
	bool valid = false;
	if ((value >= 0) && (value <= 100)) {
		double newVal = value / 100;
		valid = true;
		switch (led) {
		case (PV_LED_1):
			led_configure_brightness(idLed1, newVal);
			break;
		case (PV_LED_2):
			led_configure_brightness(idLed2, newVal);
			break;
		case (PV_LED_3):
			led_configure_brightness(idLed3, newVal);
			break;
		case (PV_LED_ALL):
			led_configure_brightness(idLed1, newVal);
			led_configure_brightness(idLed2, newVal);
			led_configure_brightness(idLed3, newVal);
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
			led_configure_fade(idLed1, value);
			break;
		case (PV_LED_2):
			led_configure_fade(idLed2, value);
			break;
		case (PV_LED_3):
			led_configure_fade(idLed3, value);
			break;
		case (PV_LED_ALL):
			led_configure_fade(idLed1, value);
			led_configure_fade(idLed2, value);
			led_configure_fade(idLed3, value);
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
			led_configure_dt(idLed1, realdt);
			break;
		case (PV_LED_2):
			led_configure_dt(idLed2, realdt);
			break;
		case (PV_LED_3):
			led_configure_dt(idLed3, realdt);
			break;
		case (PV_LED_ALL):
			led_configure_dt(idLed1, realdt);
			led_configure_dt(idLed2, realdt);
			led_configure_dt(idLed3, realdt);
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
			led_configure_flashes(idLed1, value);
			break;
		case (PV_LED_2):
			led_configure_flashes(idLed2, value);
			break;
		case (PV_LED_3):
			led_configure_flashes(idLed3, value);
			break;
		case (PV_LED_ALL):
			led_configure_flashes(idLed1, value);
			led_configure_flashes(idLed2, value);
			led_configure_flashes(idLed3, value);
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
			led_configure_period(idLed1, value);
			break;
		case (PV_LED_2):
			led_configure_period(idLed2, value);
			break;
		case (PV_LED_3):
			led_configure_period(idLed3, value);
			break;
		case (PV_LED_ALL):
			led_configure_period(idLed1, value);
			led_configure_period(idLed2, value);
			led_configure_period(idLed3, value);
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
		led_configure_time(idLed1, value);
		break;
	case (PV_LED_2):
		led_configure_time(idLed2, value);
		break;
	case (PV_LED_3):
		led_configure_time(idLed3, value);
		break;
	case (PV_LED_ALL):
		led_configure_time(idLed1, value);
		led_configure_time(idLed2, value);
		led_configure_time(idLed3, value);
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
		led_flash(idLed1);
		break;
	case (PV_LED_2):
		led_flash(idLed2);
		break;
	case (PV_LED_3):
		led_flash(idLed3);
		break;
	case (PV_LED_ALL):
		led_flash(idLed1);
		led_flash(idLed3);
		led_flash(idLed2);
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
		led_set_state(idLed1, LED_ON);
		break;
	case (PV_LED_2):
		led_set_state(idLed2, LED_ON);
		break;
	case (PV_LED_3):
		led_set_state(idLed3, LED_ON);
		break;
	case (PV_LED_ALL):
		led_set_state(idLed1, LED_ON);
		led_set_state(idLed3, LED_ON);
		led_set_state(idLed2, LED_ON);
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
		led_set_state(idLed1, LED_OFF);
		break;
	case (PV_LED_2):
		led_set_state(idLed2, LED_OFF);
		break;
	case (PV_LED_3):
		led_set_state(idLed3, LED_OFF);
		break;
	case (PV_LED_ALL):
		led_set_state(idLed1, LED_OFF);
		led_set_state(idLed3, LED_OFF);
		led_set_state(idLed2, LED_OFF);
		break;
	default:
		break;
	}
}

void PVLedPoll(void) {
	led_poll();
}

void PVStatusLedSelect(PVStatus_t led, bool state) {

	for (uint8_t i = 0; i < LED_IN_PV; i++) {
		leds_st[i] = false;
	}

	if ((led == ON_ST_1) || (led == ON_ST_12) || (led == ON_ST_13)
			|| ON_ST_ALL) {
		leds_st[0] = state;
	}

	if ((led == ON_ST_2) || (led == ON_ST_12) || (led == ON_ST_23)
			|| ON_ST_ALL) {
		leds_st[1] = state;
	}

	if ((led == ON_ST_3) || (led == ON_ST_13) || (led == ON_ST_23)
			|| ON_ST_ALL) {
		leds_st[2] = state;
	}

}
