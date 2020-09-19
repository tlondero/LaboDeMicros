/***************************************************************************//**
  @file     Button.h
  @brief    Simple  driver for buttons
  @author   MAGT
 ******************************************************************************/

#include "header/SysTick.h"
#include "header/gpio.h"
#include "header/board.h"
#include "header/timer.h"
#include "header/Button.h"


#define MAX_BUTTONS 15

static Btn active_buttons[MAX_BUTTONS];

static int8_t active_buttons_cant =-1;

// Read the enabled buttons status and performs the callback.
static void check_buttons(void);
//

//

int8_t initButton(pin_t pin ,uint8_t mode){
static int first=0;
	if (active_buttons_cant++ < MAX_BUTTONS){
		gpioMode(pin, mode);
		tim_id_t Bt_timmer = timerGetId();
		if(!first){
			timerStart(Bt_timmer,TIMER_MS2TICKS(BUTTON_REFRESH_PERIOD) ,TIM_MODE_PERIODIC , check_buttons );
			first=1;
		}
		active_buttons[active_buttons_cant].enable = true;
		active_buttons[active_buttons_cant].pin = pin;
		return active_buttons_cant;
	}
	else return -1;
}


Button_Event getButtonEvent(int8_t id){
	return active_buttons[id].ev_state;
}

void check_buttons(void){
	static uint32_t x1=0;
	int i;
	for(i=0;i<MAX_BUTTONS;i++){
		if(active_buttons[i].enable){
			active_buttons[i].pin_state = gpioRead(active_buttons[i].pin);
			if(active_buttons[i].pin_state == active_buttons[i].prev_pin_state){
				if(active_buttons[i].pin_state ==1 ){
						//aca tengo que contar X2 ms y mandar LKP
					if(++x1> LKP_THRESHOLD)
						active_buttons[i].ev_state= LKP;
					if(x1> TYPEMATIC_THRESHOLD)
						//Aca me tengo que fijar que haya pasado tantos ms y ahi empezar a mandar press
						if(x1 % TYPEMATIC_PERIOD)
						active_buttons[i].ev_state= TYPEMATIC; // Typematic equivalent
				}
				else{
					x1=0;
					active_buttons[i].ev_state= NO_EV;
				}
			}
			else{
				x1=0;
				if(active_buttons[i].pin_state == 1 ){
					active_buttons[i].ev_state= PRESS;
				}
				else{
					active_buttons[i].ev_state= RELEASE;
				}

			}
			active_buttons[i].prev_pin_state = active_buttons[i].pin_state;
		}

	}
}
