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

#define	PV_BUTTON		PORTNUM2PIN(PC,0)			//VER PIN PORQUE NO TENGO NI PUTA IDEA!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define PIN_C2_EN		PORTNUM2PIN(PC,2)			//VER PIN PORQUE NO TENGO NI PUTA IDEA!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define PIN_C7_EN		PORTNUM2PIN(PC,7)			//VER PIN PORQUE NO TENGO NI PUTA IDEA!!!!!!!!!!!!!!!!!!!!!!!!!!!

#define LED_LINE_A		PORTNUM2PIN(PC,0)			//MARRON (ESTA EN FRENTE)
#define LED_LINE_B 		PORTNUM2PIN(PA,2)           //ROJO

#define LED_IN_PV		3
#define DEC_IN_PV		2

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

static bool isEvent = false;
static PVEv_t event = NO_PV_EV;
static uint8_t listEv[NO_PV_EV] = { 0 };

//Extern leds
static int8_t idLed1;
static int8_t idLed2;
static int8_t idLed3;

//Intern leds
static bool leds_st[LED_IN_PV] = { 0 };

//Display
static int8_t dispBright;
static PVDirection_t dir;

static char *message;
static uint8_t length;
static uint8_t countMess;
static tim_id_t timer_id_mrq;
static uint32_t mrqtime = 1000;

//Timer
static tim_id_t timer_id_st;
static tim_id_t timer_open_st;
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

void multiplexLedCallback(void) {
	if (leds_st[0]) {
		gpioWrite(LED_LINE_A, HIGH);
		gpioWrite(LED_LINE_B, LOW);
	} else if (leds_st[1]) {
		gpioWrite(LED_LINE_A, LOW);
		gpioWrite(LED_LINE_B, HIGH);
	} else if (leds_st[2]) {
		gpioWrite(LED_LINE_A, LOW);
		gpioWrite(LED_LINE_B, LOW);
	} else {
		gpioWrite(LED_LINE_A, HIGH);
		gpioWrite(LED_LINE_B, HIGH);
	}
}

void dispShowText(void) {
	uint8_t i = 0;
	for (i = 0; i < 4; i++) {
		dispSendChar(i, message[i + countMess]);
	}

	switch (dir) {
	case (PV_RIGHT):
		countMess++;
		break;
	case (PV_LEFT):
		countMess--;
		break;
	case (PV_NODIR):
		dir = PV_NODIR;
		break;
	}

	if (countMess >= length) {
		countMess = 0;
	}
}

void open_animation_Callback(void) {
	static uint8_t i = 0, j;
	static char open_animation[4] = { '^', '>', '_', '<' };
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

const char* placeMayus(char *mess, uint8_t l) {
	//Pongo todo en mayusculas
	uint8_t i = 0;
	char mayusSt[l];
	for (i = 0; i < l; i++) {
		char aux = mess[i];
		if ((aux >= 97) && (aux <= 122)) {
			aux -= 32;
		}
		mayusSt[i] = aux;
	}

	const char *p = mayusSt;
	return p;
}

const char* addSpaces(char *mes, uint8_t l) {

	//Agrego 4 ' ' al principio y 4 al final
	char newMes[l + 8];
	uint8_t i = 0;
	while (i < l + 7) {
		if (i < 4) {
			newMes[i] = 32;
		} else if (i < 3 + l) {
			newMes[i] = mes[i - 4];
		} else {
			newMes[i] = 32;
		}
		i++;
	}
	newMes[l + 7] = '\0';

	const char *p = newMes;

	return p;
}

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

bool PVInit(void) {

	//Button init
	button = ButtonInit(PV_BUTTON, INPUT_PULLUP);

	//Encoder init
	idEncoder = EncoderRegister(PIN_C2_EN, PIN_C7_EN);

	//Display init
	dispInit();
	dispBright = 20;
	dir = PV_NODIR;

	led_init_driver();
	message = NULL;
	length = 0;
	countMess = 0;

	//LED STATUS INIT
	bool okLed = true;

	count = 0;
	for (uint8_t i = 0; i < DEC_IN_PV; i++) {
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
	timer_open_st = timerGetId();

	timerStart(timer_id_st, TIMER_MS2TICKS(100), TIM_MODE_PERIODIC,
			multiplexLedCallback);

	timerStart(timer_open_st, TIMER_MS2TICKS((500)), TIM_MODE_PERIODIC,
			open_animation_Callback);
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

		//message = placeMayus(mess, l);
		message = addSpaces(placeMayus(mess, l), l);

		length = checkMessageLength(message);
		countMess = 0;
	} else {
		message = NULL;
		length = 0;
		countMess = 0;
		valid = false;
	}

	timerReset(timer_id_mrq);

	return valid;
}

void PVDispMessShiftOn(void) {
	timerResume(timer_id_mrq);
}

void PVDispMessShiftOff(void) {
	timerStop(timer_id_mrq);
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
	switch (led) {
	case (ON_ST_1):
		leds_st[0] = state;
		break;
	case (ON_ST_2):
		leds_st[1] = state;
		break;
	case (ON_ST_3):
		leds_st[2] = state;
		break;
	case (ON_ST_OFF):
		break;
	default:
		break;
	}
}
