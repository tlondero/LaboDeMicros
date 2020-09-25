#include "headers/timer_tb.h"
/********************************************
 * CALLBACKS Y HELPERS
 */
void test_init(void)
{
	gpioMode(PIN_LED_RED, OUTPUT);

	gpioMode(PIN_B2, OUTPUT);
	gpioWrite(PIN_B2, LOW);

	gpioMode(PIN_B3, OUTPUT);
	gpioWrite(PIN_B3, LOW);

	timerInit();
}

#define TIMER_NOT_READY 255

void test_callback_single(void)
{
	gpioWrite(PIN_B2, LOW);
}

void test_callback_periodic(void)
{
	gpioToggle(PIN_B2);
}

static tim_id_t id_0 = TIMER_NOT_READY;
void test_callback_pause(void)
{
	static int counter_pausa = 1;

	if (counter_pausa % 10 == 0)
	{
		timerStop(id_0);
	}
	else
	{
		test_callback_periodic();
		counter_pausa++;
	}
}

static tim_id_t id_1 = TIMER_NOT_READY;
static tim_id_t id_2 = TIMER_NOT_READY;
static int counter_pause = 1;

void test_callback_pause_resume(void)
{
	gpioToggle(PIN_B2);
}

void test_callback_pause_helper(void)
{
	counter_pause++;
	gpioToggle(PIN_B3);
	if (counter_pause == 10)
	{
		timerStop(id_2);
	}
	else if (counter_pause == 20)
	{
		timerResume(id_2);
		counter_pause = 0;
	}
}

void test_callback_reset_helper(void)
{
	counter_pause++;
	gpioToggle(PIN_B3);
	if (counter_pause == 10)
	{
		timerReset(id_2);
		counter_pause = 0;
	}

}

void test_callback_reset(void)
{
	gpioToggle(PIN_B2);
}

/***********************************************
 * TEST
 ***********************************************/

/*Testeo de timer en modo bloqueante
La idea es medir el tiempo que se encuentra en HIGH el PIN_B2
*/
void test_block(pin_t pin, ttick_t delay, bool led)
{
	ttick_t ticks = TIMER_MS2TICKS(delay);
	if (led)
		gpioWrite(PIN_LED_RED, HIGH);

	gpioWrite(pin, HIGH);

	timerDelay(ticks);

	if (led)
		gpioWrite(PIN_LED_RED, LOW);

	gpioWrite(pin, LOW);
	timerDelay(ticks);
}
//Testeo de interrupciones SINGLE SHOT
static ttick_t ticks;
void test_singleshot(pin_t pin, ttick_t delay)
{
	ticks = delay;//TIMER_MS2TICKS(delay);
	if (id_0 == TIMER_NOT_READY)
	{
		gpioMode(pin, OUTPUT);
		id_0 = timerGetId();
		gpioWrite(pin, HIGH);
		timerStart(id_0, ticks, TIM_MODE_SINGLESHOT, test_callback_single);
	}
}
//Testeo de interrupciones PERIODICAS
void test_periodic(pin_t pin, ttick_t period)
{
	ticks = period; //TIMER_MS2TICKS(period);
	if (id_0 == TIMER_NOT_READY)
	{
		gpioMode(pin, OUTPUT);
		id_0 = timerGetId();
		gpioWrite(pin, HIGH);
		timerStart(id_0, ticks, TIM_MODE_PERIODIC, test_callback_periodic);
	}
}
//Testeo de pausa
void test_pausa(pin_t pin, ttick_t delay)
{
	if (id_0 == TIMER_NOT_READY)
	{
		ticks = delay;// TIMER_MS2TICKS(delay);
		gpioMode(pin, OUTPUT);
		gpioWrite(pin, HIGH);
		id_0 = timerGetId();
		timerStart(id_0, ticks, TIM_MODE_PERIODIC, test_callback_pause);
	}
}
//Testeo de reincio
static ttick_t ticks1;
static ttick_t ticks2;
void test_resume(ttick_t delay1, ttick_t delay2)
{
	if (id_1 == TIMER_NOT_READY && id_2 == TIMER_NOT_READY)
	{
		gpioMode(PIN_B2, OUTPUT);
		gpioWrite(PIN_B2, HIGH);

		ticks1 = delay1;//TIMER_MS2TICKS(delay1);
		id_1 = timerGetId();
		timerStart(id_1, ticks1, TIM_MODE_PERIODIC, test_callback_pause_helper);

		ticks2 = delay2;//TIMER_MS2TICKS(delay2);
		id_2 = timerGetId();
		timerStart(id_2, ticks2, TIM_MODE_PERIODIC, test_callback_pause_resume);
	}
}
//Testeo de reset
static ttick_t ticks;
 void test_reset(void)
 {
 	if (id_1 == TIMER_NOT_READY && id_2 == TIMER_NOT_READY)
 	{
 		ticks = 314; //ms

 		gpioMode(PIN_B2, OUTPUT);
 		gpioWrite(PIN_B2, HIGH);

 		id_1 = timerGetId();
 		timerStart(id_1, ticks, TIM_MODE_PERIODIC, test_callback_reset_helper);
 		//id_2 timer will be reseted
 		id_2 = timerGetId();
 		timerStart(id_2, ticks, TIM_MODE_SINGLESHOT, test_callback_reset);
 	}
 }

void run_tests(void)
{

	//Testing blocking delay timer
	test_block(PIN_B2, 314, false); //Passing

	//Testing Single Shot
	//test_singleshot(PIN_B2, 314); //Passing

	//Test periodic
	//test_periodic(PIN_B2, 314); //Passing

	//Test Pausa
	//test_pausa(PIN_B2, 314); //Passing

	//Test Pausa y Resume
	//test_resume(500, 314); //Passing

	//Test de Reset
	//test_reset();
}
