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
#define HIGH 1
#define LOW 0

#define LEN(array) sizeof(array) / sizeof(array[0])

#if DEVELOPMENT_MODE
#define DEBUG_PIN PIN_B10
#endif

/*******************************************************************************
 * STRUCTS AND TYPEDEFS
 ******************************************************************************/
typedef struct
{
  pin_t pin_A;
  pin_t pin_B;
  bool event_flag;
  event_t event_queue[20];
  uint8_t in_pointer;
  uint8_t out_pointer;
} encoder_state_t;

/*******************************************************************************
 * STATIC VARIABLES
 ******************************************************************************/
static encoder_state_t encoders[MAX_ENCODERS];
static uint16_t enconders_cant = 0;
static tim_id_t encoder_timer_id;


/*******************************************************************************
 * FUNCTION DECLARATIONS
 ******************************************************************************/
//Adds a new a event to the queue
void EncoderAddNewEvent(encoder_id id, event_t ev);
//Update encoders
void EncoderUpdate(void);

/*******************************************************************************
 * FUNCTION DEFINITIONS
 ******************************************************************************/

//Adds new encoder
encoder_id EncoderRegister(pin_t pin_A, pin_t pin_B)
{
	encoder_id id;

	id = enconders_cant++;

	encoders[id].pin_A = pin_A;
	encoders[id].pin_B = pin_B;
	encoders[id].event_flag = false;

	//Init circular buffer
	encoders[id].in_pointer = 0;
	encoders[id].out_pointer = 0;

	gpioMode(encoders[id].pin_B, INPUT_PULLUP);
	gpioIRQ(encoders[id].pin_B, GPIO_IRQ_MODE_FALLING_EDGE, EncoderUpdate);
	gpioMode(encoders[id].pin_A, INPUT_PULLUP);

	return id;
}


//Init encoder driver
void EncoderInit(void)
{
	static bool warm_up_rdy = false;
	if (!warm_up_rdy)
	{
		warm_up_rdy = true;
		timerInit();
		encoder_timer_id = timerGetId();
		timerStart(encoder_timer_id, 60, TIM_MODE_SINGLESHOT, NULL); //Previene rebotes

#if DEVELOPMENT_MODE
		gpioMode(DEBUG_PIN, OUTPUT);
		gpioWrite(DEBUG_PIN, LOW);
#endif
	}
}
//Update encoders
void EncoderUpdate(void)
{
	encoder_id id = 0;
	if (timerExpired(encoder_timer_id))
	{

#if DEVELOPMENT_MODE
		gpioWrite(DEBUG_PIN, HIGH);
#endif
		for (id = 0; id < enconders_cant; id++)
		{
			//COMPARE PREV AND CURRENT STATE
			if (gpioRead(encoders[id].pin_A) == HIGH)
			{
				EncoderAddNewEvent(id, RIGHT_TURN);
			}
			else if(gpioRead(encoders[id].pin_A) == LOW)
			{
				EncoderAddNewEvent(id, LEFT_TURN);
			}
		}
		timerReset(encoder_timer_id);

#if DEVELOPMENT_MODE
		gpioWrite(DEBUG_PIN, LOW);
		int a = -1;
#endif
	}
}
//Reports if a new event is available
bool EncoderEventAVB(encoder_id id)
{
	return encoders[id].event_flag;
}
//Circular event buffer
//Adds a new a event to the queue
void EncoderAddNewEvent(encoder_id id, event_t ev)
{
	if(encoders[id].in_pointer  != (encoders[id].out_pointer + LEN(encoders[id].event_queue))){
		encoders[id].event_queue[encoders[id].in_pointer] =  ev;
		encoders[id].in_pointer = (++encoders[id].in_pointer) % (LEN(encoders[id].event_queue));
		encoders[id].event_flag = EVENT_AVB;
	}

}
//Pops the last event in the queue
event_t EncoderPopEvent(encoder_id id)
{
	//Is the buffer non-empty?
	if (encoders[id].out_pointer != encoders[id].in_pointer)
	{
		event_t ev;
		ev = encoders[id].event_queue[encoders[id].out_pointer];
		encoders[id].out_pointer = (++encoders[id].out_pointer) % (LEN(encoders[id].event_queue));

		return ev;
	}
	else
	{
		encoders[id].event_flag = EVENT_NOT_AVB;
	}
}
