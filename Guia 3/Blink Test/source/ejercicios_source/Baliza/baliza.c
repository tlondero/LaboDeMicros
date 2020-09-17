#include "header/ejercicios_headers/Baliza/baliza.h"

unsigned long ms2UL(unsigned long miliseconds){
	unsigned long res = (unsigned long)(14300*miliseconds);
	return res;
}
/* Parametros:
 * number: unsigned long
 * 			recibe un número expresado en UL y devuelve su equivalente en milisengundos
*/
unsigned long UL2ms(unsigned long number){
	unsigned long milisec = (unsigned long)(number/14300);
	return milisec;
}



void init_baliza(void){
	gpioMode(PIN_LED_BLUE, OUTPUT);
	gpioWrite(PIN_LED_BLUE, HIGH);


	gpioMode(PIN_LED_RED, OUTPUT);
	gpioWrite(PIN_LED_RED, HIGH);

	gpioMode(PIN_SW3, INPUT_PULLUP);

	//PIN_LED_AMA_EXT
	gpioMode(PIN_LED_AMA_EXT, OUTPUT);
	gpioWrite(PIN_LED_AMA_EXT, HIGH);

}

void delayLoop(uint32_t veces)
{
    while (veces--);
}


void update_baliza(int periodo){

	static int delay_acumulado = 0;

	if ( delay_acumulado < ms2UL((unsigned long )(periodo/2)) ){
		delayLoop(ms2UL(1));
		delay_acumulado += ms2UL(1);
	}
	else{
		gpioToggle(PIN_LED_BLUE);
		delay_acumulado = 0;
	}
}

void do_baliza(void){

	int prev;
	static int current = HIGH;
	static bool baliza_state = false;
	prev = current;
	current = gpioRead(PIN_SW3);//gpioRead(PIN_SW2);//gpioRead(PIN_SW3);

	if ( current==HIGH && prev==LOW){
		//delayLoop(40000UL); #Para
		baliza_state = !baliza_state;
		gpioWrite(PIN_LED_RED, !baliza_state);
		gpioWrite(PIN_LED_BLUE, !baliza_state);
	}

	if ( baliza_state == true ){
		update_baliza(2000);
	}
}




