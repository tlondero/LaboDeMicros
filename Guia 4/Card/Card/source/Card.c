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
#define CHAR_LENGHT (5)
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
static bool error;
/*******************************************************************************
 * FUNCTION PROTOTYPES WITH FILE SCOPE DECLARATION
 ******************************************************************************/
//Callback
void cardCallback(void);
//	printf("Hola papi");
void enableCallback(void);
void clockCallback(void);
bool checkParity(void);
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
	gpioIRQ(CARD_CLK_PIN, GPIO_IRQ_MODE_FALLING_EDGE, clockCallback);
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
void clockCallback(void) { // seguro en algun mometno debería buscar los SS, FS y ES
	static uint8_t bit_counter = 0;
	static uint8_t character = 0;
	static uint8_t char_counter = 0;
	bool my_data = gpioRead(CARD_DATA_PIN)
	if (enable) {
		if (bit_counter < CHAR_LENGHT) {
			character = character | (my_data << bit_counter);
		} else {
			bit_counter = 0;
			data[char_counter++] = character;
			character = 0;
			if(data[0] != SS){
				error = true; //CHEQUEAR
			}
		}
		if (char_counter == DATA_LENGHT - 1) {
			char_counter = 0;
			if ((!checkParity())) {
				//si no da la paridad aca podes hace cosas
			}
		}
	}
}

bool checkParity(void){
	uint8_t i=0,j;
	uint8_t char_parity=false;
	uint8_t lrc_parity = false;
	bool is_ok;
	for(i=0;i< DATA_LENGHT-2 ; i++){//Stop before the LRC bit
		for (j=0; j< CHAR_LENGHT ; j++){
			char_parity ^=  ((data[i] & (1<<j))>>j);//we test the parity of the chars
			lrc_parity ^=  ((data[i] & (1<<j))>>j);
		}
		if(char_parity == 0){ //if it wasnt ODD parity.
			error=true;
		}
		char_parity = false;
	}
	if (lrc_parity ==data[DATA_LENGHT-1]){//CHEQUEAR
		//no me quedo del todo claro como es la pariddad del LRC
		is_ok=true;
	}
	else {
		is_ok=false;
	}
	return is_ok;
}
