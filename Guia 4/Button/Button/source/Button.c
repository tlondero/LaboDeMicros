/***************************************************************************//**
  @file     Button.h
  @brief    Simple  driver for buttons
  @author   MAGT
 ******************************************************************************/

#include "header/SysTick.h"
#include "header/gpio.h"
#include "header/board.h"
#include "header/Button.h"
#include "timer.h"
//#include "timer.h"

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
			timerInit();
		}
		active_buttons[active_buttons_cant].enable = true;
		active_buttons[active_buttons_cant].pin = pin;
		active_buttons[active_buttons_cant].prev_pin_state = HIGH;
		return active_buttons_cant;
	}
	else return -1;
}

Button_Event getButtonEvent(int8_t id){
	Button_Event ret = active_buttons[id].ev_state;
	active_buttons[id].ev_state = NO_EV;
#if VISIBLE_LEDS_EVENTS
	active_buttons[id].ev_state = ret;
#endif
	return ret;
}


void setIRQ_button(int8_t id, uint8_t IRQMode, void*(fcallback)(void)){
	gpioIRQ(active_buttons[id].pin, IRQMode, fcallback);
}

void LKP_or_Typematic_mode(int8_t id, bool mode){
	active_buttons[id].lkp = mode;
}

void check_buttons(void){
	static uint32_t x1[MAX_BUTTONS]={0};
	int i;
	for(i=0;i<MAX_BUTTONS;i++){
		if(active_buttons[i].enable){
			active_buttons[i].pin_state = gpioRead(active_buttons[i].pin);
			if(active_buttons[i].pin_state == active_buttons[i].prev_pin_state){
				if(active_buttons[i].pin_state == 0 ){
					x1[i]= x1[i]+1;
					if(((x1[i] > TYPEMATIC_THRESHOLD) && (x1[i] % TYPEMATIC_PERIOD)) )
								active_buttons[i].ev_state= PRESS; // Typematic equivalent
					else if((x1[i] > LKP_THRESHOLD) && (x1[i] < LKP_THRESHOLD+2) )
												active_buttons[i].ev_state= LKP;
					else
								active_buttons[i].ev_state= NO_EV;
				}
				else{
					x1[i]=0;
					active_buttons[i].ev_state= NO_EV;
				}
			}
			else{
				if(active_buttons[i].pin_state == 0 ){
					active_buttons[i].ev_state= PRESS;
					timerDelay(10);
				}
				else{
					active_buttons[i].ev_state= RELEASE;
					timerDelay(10);
				}
				if((x1[i] > 0)&&(x1[i] < LKP_THRESHOLD))
					active_buttons[i].ev_state= SKP;
				x1[i]=0;

			}
			active_buttons[i].prev_pin_state = active_buttons[i].pin_state;
		}

	}
}
