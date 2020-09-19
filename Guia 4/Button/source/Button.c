/***************************************************************************//**
  @file     Button.h
  @brief    Simple  driver for buttons
  @author   MAGT
 ******************************************************************************/
#include "Systick.h"
#include "gpio.h"
#include "board.h"
#include "timer.h"

#define MAX_BUTTONS 15

static Btn active_buttons[MAX_BUTTONS];

static int8_t active_buttons_cant =-1;

// Read the enabled buttons status and performs the callback.
static void check_buttons(void);
//

//

int8_t initButton(pin_t pin ,uint8_t mode, bool LKP_or_Typemattic){
	if (active_buttons_cant++ < MAX_BUTTONS){
		gpioMode(pin, mode);
		Bt_timmer = timerGetId();
		timerStart(Bt_timmer,Time_in_ms ,TIM_MODE_PERIODIC , check_buttons )
		//we call this just once
		active_buttons[active_buttons_cant].enable = true;
		active_buttons[active_buttons_cant].pin = pin;
		active_buttons[active_buttons_cant].lkp =  LKP_or_Typemattic;
		return active_buttons_cant;
	}
	else return -1;
}


Button_Event getButtonEvent(int8_t id){
	return active_buttons[id].ev_state;
}

void check_buttons(void){
	int i;
	for(i=0;i<MAX_BUTTONS;i++){
		if(active_buttons[i].enable){
			active_buttons[i].pin_state = gpioRead(active_buttons[i].pin);
			if(active_buttons[i].pin_state == active_buttons[i].prev_pin_state){
				if(active_buttons[i].pin_state ==1 ){//aca seria casos de LKP y TYPEMATIC o de NONE
					if(active_buttons[i].lkp)
						active_buttons[i].ev_state= LKP;
					else
						active_buttons[i].ev_state= PRESS; // Typematic equivalent
				}
				else{
					active_buttons[i].ev_state= NO_EV;
				}
			}
			else{
				if(active_buttons[i].pin_state == 1 ){//aca seria casos de LKP y TYPEMATIC o de NONE
					active_buttons[i].ev_state= PRESS;
				}
				else{
					active_buttons[i].ev_state= RELEASE;
				}
				//ACA SERIA PRESSED O RELEASE
			}

		}

	}
}
