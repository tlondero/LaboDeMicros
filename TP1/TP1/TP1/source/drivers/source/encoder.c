#include "../headers/encoder.h"
#include "../headers/timer.h"
#include "../headers/board.h"
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define MAX_ENCODERS 5
#define NO_MORE_ENCODERS 255
#define DEVELOPMENT_MODE 1
/*******************************************************************************
 * STATIC VARIABLES
 ******************************************************************************/
static encoder_state_t encoders[MAX_ENCODERS];
static uint16_t enconders_cant = 0;

/*******************************************************************************
 * FUNCTION DEFINITIONS
 ******************************************************************************/

static tim_id_t encoder_timer_id;
void do_nothing(void){
	static int a = 0;
	a++;
	return;
}
void encoder_init(void)
{
    static bool warm_up_rdy = false;
    if (!warm_up_rdy)
    {
        warm_up_rdy = true;
        timerInit();
        encoder_timer_id = timerGetId();
        timerStart(encoder_timer_id, 60, TIM_MODE_SINGLESHOT, do_nothing);//
        //timerStop(encoder_timer_id);
        gpioMode(PIN_B10, OUTPUT);
        gpioWrite(PIN_B10,LOW);
    }
}

encoder_id encoder_register(pin_t pin_A, pin_t pin_B)
{
	encoder_id id;

	id = enconders_cant++;
	encoders[id].pin_A = pin_A;
	encoders[id].pin_B = pin_B;
	encoders[id].current_state_A = HIGH;
	encoders[id].current_state_B = HIGH;
	encoders[id].prev_state_A = encoders[id].current_state_A;
	encoders[id].prev_state_A = encoders[id].current_state_B;
	encoders[id].finite_count = FINITE; // default
	encoders[id].max = 1000;
	encoders[id].min = 1;
	encoders[id].clockwise = CLOCKWISE;
	encoders[id].count = encoders[id].min;

	gpioMode(pin_A, INPUT_PULLDOWN);
	gpioIRQ(pin_A, GPIO_IRQ_MODE_LOW_STATE, encoder_update);
	gpioMode(pin_B, INPUT_PULLDOWN);
	//gpioIRQ(pin_B, GPIO_IRQ_MODE_LOW_STATE, encoder_update);

    return id;
}

void encoder_set_maxmin(encoder_id id, int16_t min, int16_t max)
{
#if DEVELOPMENT_MODE
    if (id < enconders_cant)
#endif
    {
        encoders[id].max = max;
        encoders[id].min = min;
    }
}

void encoder_reset_cnt(encoder_id id)
{
#if DEVELOPMENT_MODE
    if (id < enconders_cant)
#endif
    {
        encoders[id].count = encoders[id].min;
    }
}

uint32_t encoder_get_count(encoder_id id)
{
	return encoders[id].count;
}

//Callback for timer
void encoder_update(void)
{
    int i = 0;
    if (timerExpired(encoder_timer_id)){
#if DEVELOPMENT_MODE
    	gpioWrite(PIN_B10,HIGH);
#endif
		for (i = 0; i < enconders_cant; i++)
		{
			//READ GPIOS INPUT PINS and UPDATE CURRENT STATE
			encoders[i].current_state_A = gpioRead(encoders[i].pin_A);
			encoders[i].current_state_B = gpioRead(encoders[i].pin_B);
			//COMPARE PREV AND CURRENT STATE
			if (encoders[i].current_state_B == HIGH)
			{
				if(encoders[i].count < encoders[i].max)
					encoders[i].count += 1;
			}
			else
			{
				if(encoders[i].count > encoders[i].min)
					encoders[i].count -= 1;
			}
		}
	    timerReset(encoder_timer_id);
#if DEVELOPMENT_MODE
	    gpioWrite(PIN_B10,LOW);
#endif
    }

}
