/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "board.h"
#include "gpio.h"
#include "stdbool.h"
#include <stdio.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void delayLoop(uint32_t veces);

//static void guido_delay(double period);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/*******************************************************************************
 * BALIZA
 ******************************************************************************/

static void update_baliza(int period);
static unsigned long human2UL(int number);
static int UL2human(unsigned long numer);

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	gpioMode(PIN_LED_BLUE, OUTPUT);
	gpioWrite(PIN_LED_BLUE, HIGH);

	gpioMode(PIN_LED_RED, OUTPUT);
	gpioWrite(PIN_LED_RED, HIGH);

	gpioMode(PIN_SW3, INPUT);

	//Con SW2
	gpioMode(PIN_SW2, INPUT_PULLUP);

	//Prender el led sobre la protoboard
	gpioMode(PIN_PULSADOR, INPUT_PULLUP);
	gpioMode(PIN_LED_AMA, OUTPUT);
	//gpioWrite(PIN_LED_AMA, HIGH);

	//BALIZA

}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	//Baliza
	static bool baliza_state = false;
	int prev;
	static int current = HIGH;
	prev = current;
	current = gpioRead(PIN_PULSADOR);//gpioRead(PIN_SW2);//gpioRead(PIN_SW3);

	if ( current==HIGH && prev==LOW){
		//delayLoop(40000UL); #Para
		baliza_state = !baliza_state;
		gpioWrite(PIN_LED_RED, !baliza_state);
		gpioWrite(PIN_LED_AMA, baliza_state);
	}

	if ( baliza_state == true ){
		update_baliza(2000);
	}


















	//Pul2Switch
	//Prende y apaga el led azul usando SW3 como switch

	/*static int current = HIGH;
	int prev;
	prev = current;
	current = gpioRead(PIN_PULSADOR);//gpioRead(PIN_SW2);//gpioRead(PIN_SW3);

	if ( current==HIGH && prev==LOW){
		delayLoop(40000UL);
		gpioToggle(PIN_LED_AMA);					//gpioToggle(PIN_LED_BLUE);
		//gpioWrite(PIN_LED_AMA, HIGH);
	}
	int check =  gpioRead(PIN_LED_AMA);
	*/

	//printf("hola");


    //delayLoop(4000000UL);
	//guido_delay(2);
    //gpioToggle(PIN_LED_RED);
    //guido_delay();
    //gpioToggle(PIN_LED_BLUE);
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
/*
static void guido_delay(double period){
	double wait = period / 2.0;
	time_t start, end;
	time(&start);
	do time(&end); while(difftime(end, start) <= wait);
}*/

//71500 UL = 5ms
static void delayLoop(uint32_t veces)
{
    while (veces--);
}

static void update_baliza(int periodo){

	static int delay_acumulado = 0;

	if ( delay_acumulado < human2UL((int)(periodo/2)) ){
		delayLoop(human2UL(1));
		delay_acumulado += human2UL(1);
	}
	else{
		gpioToggle(PIN_LED_AMA);
		delay_acumulado = 0;
	}
}

/* Parametros:
 * number: int
 * 			recibe un número expresado en milisengundos y devuelve su equivalente en UL
*/
static unsigned long human2UL(int miliseconds){
	unsigned long res = 14300*miliseconds;
	return res;
}
/* Parametros:
 * number: unsigned long
 * 			recibe un número expresado en UL y devuelve su equivalente en milisengundos
*/
static int UL2human(unsigned long number){
	int milisec = number/14300;
	return milisec;
}



/*******************************************************************************
 ******************************************************************************/
