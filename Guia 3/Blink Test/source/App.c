/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

//#include <header/ejercicios_headers/BalizaSwitchSysTick/balizaSwitchSysTick.h>
//#include "header/ejercicios_headers/Baliza/baliza.h"
//#include "header/ejercicios_headers/BalizaSysTick/balizast.h"
#include "header/board.h"
#include "header/gpio.h"
#include "stdbool.h"
#include <stdio.h>
#include "header/SysTick.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void delayLoop(uint32_t veces);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 *******************************************************************************
 ******************************************************************************/


/*******************************************************************************
 * INTERRUPCIONES
 ******************************************************************************/

static CallbackInterrupt BlinkIRQ;
static CallbackInterrupt SwitchCheckIRQ;
static CallbackInterrupt BalizaSystick;
static CallbackInterrupt  BlankInterrupt;



/*******************************************************************************
 * BALIZA
 ******************************************************************************/

//static void update_baliza(int period);
static unsigned long ms2UL(int number);
static int UL2ms(unsigned long numer);

void toggle_led_ama(void){
	gpioToggle(PIN_LED_AMA_EXT);
}

void App_Init (void)
{
	//Con SW2
	//gpioMode(PIN_SW2, INPUT_PULLUP);

	//Prender el led sobre la protoboard
	//gpioMode(PIN_PULSADOR, INPUT_PULLUP);
	//gpioMode(PIN_LED_AMA, OUTPUT);
	//gpioWrite(PIN_LED_AMA, HIGH);

	//BALIZA SIN INTERRUPCIONES
	//SE ENCIENDE LA BALIZA CUANDO PRESIONAMOS EL BOTON

	//Descomentar para correr
	//init_baliza;

	//*********************************************//

	// Systick baliza (Actualiza y cheque el boton
	// en el momento de la interrupción

	/*init_baliza();
	BalizaSystick.enable = true;
	BalizaSystick.periodo = 1000;
	BalizaSystick.stick_callback = do_baliza_systick;
	CallbackInterrupt *irqs[] = {&BalizaSystick};
	SysTick_Init(irqs,1);
	*/

	//*********************************************//
	// Systick baliza y pulsador
	// Se utiliza la interrupción periodica para
	// chequear el pulsador y togglear la baliza
	// utilizando un contador que contabiliza
	// la cantidad de veces que se ha disparado la interrupción.
	// Se utiliza como base de tiempo
	/*
	init_baliza_3();
	BlinkIRQ.enable = true;
	BlinkIRQ.periodo = 1000;
	BlinkIRQ.stick_callback = toggle_baliza_3;

	SwitchCheckIRQ.enable = true;
	SwitchCheckIRQ.periodo = 125;
	SwitchCheckIRQ.stick_callback = do_baliza_systick_3;

	BlankInterrupt.stick_callback = NULL;
	CallbackInterrupt *irqs[] = {&BlinkIRQ, &SwitchCheckIRQ, &BlankInterrupt};
	SysTick_Init(irqs,2);
	*/

	//Interrupciones de pines
	gpioMode(PIN_LED_AMA_EXT, OUTPUT);
	gpioWrite(PIN_LED_AMA_EXT,HIGH);
	//gpioMode(PIN_SW3, INPUT);
	//gpioIRQ(PIN_SW3, GPIO_IRQ_MODE_RISING_EDGE, toggle_led_ama);
	gpioMode(PIN_B2, INPUT);
	gpioIRQ(PIN_B2, GPIO_IRQ_MODE_BOTH_EDGES, toggle_led_ama);

	gpioMode(PIN_B3, INPUT);
	gpioIRQ(PIN_B3, GPIO_IRQ_MODE_BOTH_EDGES, toggle_led_ama);

}

/* Función que se llama constantemente en un ciclo infinito */

void App_Run (void)
{
	//1-Do baliza sin interrupciones
	//do_baliza();

	//2-Do baliza con interrupciones
	//no code

	//3-Do baliza y chequeo del pulsador con interrupciones
	//no code




}


