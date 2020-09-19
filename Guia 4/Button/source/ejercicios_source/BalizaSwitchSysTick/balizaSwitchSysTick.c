#include <header/ejercicios_headers/balizaSwitchSysTick/balizaSwitchSysTick.h>

static bool baliza_state = false;

void do_baliza_systick_3(void){
	int prev;
	static int current = HIGH;
	prev = current;
	current = gpioRead(PIN_SW3);//gpioRead(PIN_SW2);//gpioRead(PIN_SW3);


	if ( current==HIGH && prev==LOW ){
		//delayLoop(40000UL); #Para
		baliza_state = !baliza_state;
		gpioWrite(PIN_LED_RED, !baliza_state);
		gpioWrite(PIN_LED_BLUE, !baliza_state);
		gpioWrite(PIN_LED_AMA_EXT, !baliza_state);
	}
}

void toggle_baliza_3(void){
	if (baliza_state){
		gpioToggle(PIN_LED_BLUE);
		gpioToggle(PIN_LED_AMA_EXT);
	}

}

void init_baliza_3(void){
	gpioMode(PIN_LED_BLUE, OUTPUT);
	gpioWrite(PIN_LED_BLUE, HIGH);


	gpioMode(PIN_LED_RED, OUTPUT);
	gpioWrite(PIN_LED_RED, HIGH);

	gpioMode(PIN_SW3, INPUT_PULLUP);

	//PIN_LED_AMA_EXT
	gpioMode(PIN_LED_AMA_EXT, OUTPUT);
	gpioWrite(PIN_LED_AMA_EXT, HIGH);

}

