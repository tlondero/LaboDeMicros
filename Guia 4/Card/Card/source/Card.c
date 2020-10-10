/***************************************************************************//**
 @file     Card.c
 @brief    magnetic card xd
 @author   MAGT
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "header/Card.h"
#include "header/gpio.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ********************************************************************************/
#define PAN_LENGHT (19)
#define DATA_LENGHT (40)
#define CARD_KINETIS_PIN (0)
#define FS 0b01101
#define SS 0b01011
#define ES 0b11111
#define LOW_NYBBLE_MASK (0b00001111)
#define CARD_EN_PIN 0
#define CARD_DATA_PIN 0
#define CARD_CLK_PIN 0

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
static uint8_t pan[PAN_LENGHT];
static uint8_t data[DATA_LENGHT]; //?
static bool enable;
/*******************************************************************************
 * FUNCTION PROTOTYPES WITH FILE SCOPE DECLARATION
 ******************************************************************************/
//Callback
void cardCallback(void);
//	printf("Hola papi");
void enableCallback(void);
void clockCallback(void);
/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE DEFINITION
 ******************************************************************************/
//void cardSetCallback(void (*callback)(void)) {
//	gpioIRQ(CARD_KINETIS_PIN, GPIO_IRQ_MODE_FALLING_EDGE, callback);
//}
void cardInitDriver(void (*fun_callback)(void)) {
	gpioMode(CARD_EN_PIN, INPUT);
	gpioMode(CARD_DATA_PIN, INPUT);
	gpioMode(CARD_CLK_PIN, INPUT);
	////////////////////////////////////////////////////////////////////////////////////
	gpioIRQ(CARD_EN_PIN, GPIO_IRQ_MODE_BOTH_EDGES, enableCallback);
	gpioIRQ(CARD_CLK_PIN, GPIO_IRQ_MODE_RISING_EDGE, clockCallback);
	//ACA le puse un edge cualquiera al clock, habría que pensarlo, no se si la info es
	//en el flanco positivo o negativo
	//No le puse interrupcion a la data porque ni idea si debería, porque con el enable empieza
	//toda la idea de la trama
	////////////////////////////////////////////////////////////////////////////////////
	NVIC_EnableIRQ(PORTC_IRQn);
	//ESTO DEPENDE DE QUE PUERTO USEMOS

	cardCallback = fun_callback;
}

uint8_t * cardGetPAN(void) {
	uint8_t i = 0;
	while ((data[i + 1] != FS) && (i < PAN_LENGHT)) {
		pan[i] = data[i + 1] & LOW_NYBBLE_MASK;
		i++;
	}
	return &pan[0];
}
void enableCallback(void) {
	enable = gpioRead(CARD_EN_PIN);
	if (enable) {
		//seguro habrá que hacer algo si estoy enable tipo configuracion
	} else {
		//y alguna otra cuando termine
	}
}
void clockCallback(void) {
	if (enable) {
		//aca agarraríamos la data de la tarjeta
	}
}
