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

Button_Event getButtonEvent (int8_t id){
	static uint32_t x1[MAX_BUTTONS]={0};
	Button_Event ret;
	ret.hold_evs = NO_EV;
	ret.inst_evs = NO_EV;


	if(active_buttons[id].newinfo){
	if(active_buttons[id].pin_state == active_buttons[id].prev_pin_state){
		if(active_buttons[id].pin_state == 0 ){
			x1[id]= x1[id]+1;
			if(((x1[id] > TYPEMATIC_THRESHOLD) && (x1[id] % TYPEMATIC_PERIOD)) )
				ret.inst_evs= PRESS; // Typematic equivalent
			if((x1[id] > LKP_THRESHOLD) && (x1[id] < LKP_THRESHOLD+2) )
				ret.hold_evs= LKP;
		}
		else{
			x1[id]=0;
			}
	}
	else{
		if(active_buttons[id].pin_state == 0 ){
			ret.inst_evs= PRESS;
//			timerDelay(10);
		}
		else{
			ret.inst_evs= RELEASE;
		//	timerDelay(10);
		}
		if((x1[id] > 0)&&(x1[id] < LKP_THRESHOLD))
			ret.hold_evs= SKP;
		x1[id]=0;

		}
	}
	active_buttons[id].prev_pin_state = active_buttons[id].pin_state;
	active_buttons[id].newinfo= false;
	return ret;
}


void setIRQ_button(int8_t id, uint8_t IRQMode, pinIrqFun_t fcallback){
	gpioIRQ(active_buttons[id].pin, IRQMode, fcallback);
}


void check_buttons(void){
	int i;
	for(i=0;i<MAX_BUTTONS;i++){
		if(active_buttons[i].enable){
			active_buttons[i].newinfo = true;
			active_buttons[i].pin_state = gpioRead(active_buttons[i].pin);
		}

	}
}
