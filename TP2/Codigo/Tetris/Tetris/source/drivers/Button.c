/***************************************************************************//**
 @file     Button.h
 @brief    Simple  driver for buttons
 @author   MAGT
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <../header/drivers/Button.h>
#include <../header/drivers/gpio.h>
#include <../header/drivers/SysTick.h>
#include <../header/drivers/timer.h>
#include "../header/board.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define MAX_BUTTONS 10
#define BUFF_LEN 50
/*******************************************************************************/
/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
typedef struct {
	pin_t pin;
	bool enable;
	bool new_info; //flag for the callback to announce new information
	uint8_t gpio_buffer[BUFF_LEN];
} Btn;

static Btn active_buttons[MAX_BUTTONS];

static ButtonEvent ev_buffer[MAX_BUTTONS][BUFF_LEN];

static int8_t active_buttons_cant = -1;

static uint32_t selector[BUFF_LEN] = { 1 };
//Selector, indicates the amount of gpio events between ButtonGetEvent Calls
/*******************************************************************************
 * FUNCTION DEFINITION WITH GLOBAL SCOPE
 ******************************************************************************/
// Read the enabled buttons status and performs the callback.
static void ButtonsCheck(void);
//
//

int8_t ButtonInit(pin_t pin, uint8_t mode) {
	static int first = 0, i;
	if (active_buttons_cant++ < MAX_BUTTONS) {
		gpioMode(pin, mode); //Inits the gpio for the pin
		if (!first) { //if is the first time intis the timer and the periodic interruption for polling
			tim_id_t Bt_timmer = timerGetId();
			timerStart(Bt_timmer, TIMER_MS2TICKS(BUTTON_REFRESH_PERIOD),
					TIM_MODE_PERIODIC, ButtonsCheck);
			first = 1;
			timerInit();
		}

		for (i = 0; i < BUFF_LEN; i++) {
			active_buttons[active_buttons_cant].gpio_buffer[i] = 0; //The gpio buffer starts with zeros
		}
		active_buttons[active_buttons_cant].enable = true;
		active_buttons[active_buttons_cant].pin = pin;
		return active_buttons_cant;
	} else
		return -1; //This is the case where  you've reached the max ammount of buttons
}

const ButtonEvent * ButtonGetEvent(int8_t id) {
	static uint32_t time_counter[MAX_BUTTONS] = { 0 };
	int i = 0, ev_counter = 0;
	if (active_buttons[id].new_info) { //if the buttons is enabled
		for (i = 0; i < selector[id]; i++) { // for every state in the GPIO buffer
			if (active_buttons[id].gpio_buffer[i + 1]
					== active_buttons[id].gpio_buffer[i]) {  //if they are equal  you could get a typemattic event or a LKP event
				if (active_buttons[id].gpio_buffer[i + 1] == 0) {
					time_counter[id] = time_counter[id] + 1;
					if (((time_counter[id] > TYPEMATIC_THRESHOLD)
							&& (time_counter[id] % TYPEMATIC_PERIOD))) {//evert TYPEMATIC_PERIOD you get a press event if you satisfy the time criteria
						ev_buffer[id][ev_counter++] = PRESS; // Typematic equivalent
					}

					if ((time_counter[id] > LKP_THRESHOLD) //If the time passed is greather than the threshold you get one LKP event
							&& (time_counter[id] < LKP_THRESHOLD + 2)) {
						ev_buffer[id][ev_counter++] = LKP;
					}
				} else {
					time_counter[id] = 0;
				}
			} else {
				if (active_buttons[id].gpio_buffer[i + 1] == 0) {
					ev_buffer[id][ev_counter++] = PRESS; //if went from down to up
				} else {
					ev_buffer[id][ev_counter++] = RELEASE; //if went from up to down
				}
				if ((time_counter[id] > 0)
						&& (time_counter[id] < LKP_THRESHOLD) && (active_buttons[id].gpio_buffer[i + 1] == 1))
					ev_buffer[id][ev_counter++] = SKP; // up to down in a time shorter than LKP_THRESHOLD
				//one could change this define to a SKP_THRESHOLD and diffirentiate more levels.
				time_counter[id] = 0; //reset the time_counter for the hold events.
			}
		}
		ev_buffer[id][ev_counter] = EOQ;//we add the End Of Queue
	} else {
		ev_buffer[id][ev_counter] = EOQ; //
	}

	active_buttons[id].gpio_buffer[0] = active_buttons[id].gpio_buffer[i];
	active_buttons[id].new_info = false;
	return (const ButtonEvent *) &(ev_buffer[id][0]); //this is done on purpose
}

void ButtonSetIRQ(pin_t id, uint8_t IRQ_mode, pinIrqFun_t fcallback) {
	gpioIRQ(active_buttons[id].pin, IRQ_mode, fcallback);
}

void ButtonsCheck(void) {

	int i;
	for (i = 0; i < MAX_BUTTONS; i++) {
		if (active_buttons[i].enable) {
			if (active_buttons[i].new_info == false) { //if there is no new info
				active_buttons[i].new_info = true;     // Get up the new_info flag{
				active_buttons[i].gpio_buffer[1] = gpioRead( //and add the new value to the second position of hte buffer
						active_buttons[i].pin);
				//the first one is reserved for the last state of the last run
				selector[i] = 1; //and set the gpio buffer sisze to 1
			} else {
				if (selector[i] < BUFF_LEN) {
					selector[i] = selector[i] + 1; //increase the gpio buffer size indicator
					active_buttons[i].gpio_buffer[selector[i]] = gpioRead( //and read the new state
							active_buttons[i].pin);
				}
			}

		}

	}
}

