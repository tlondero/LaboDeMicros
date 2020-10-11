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
#include "MK64F12.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ********************************************************************************/

#define DATA_LENGHT (40)
#define CHAR_LENGHT (5)
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
//COUNTERS
static uint8_t bit_counter = 0;
static uint8_t character = 0;
static uint8_t char_counter = 0;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH FILE SCOPE DECLARATION
 ******************************************************************************/
//Callback
void cardCallback(void);

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
	gpioMode(CARD_EN_PIN, INPUT_PULLUP);
	gpioMode(CARD_DATA_PIN, INPUT_PULLUP);
	gpioMode(CARD_CLK_PIN, INPUT_PULLUP);
	////////////////////////////////////////////////////////////////////////////////////
	gpioIRQ(CARD_EN_PIN, GPIO_IRQ_MODE_BOTH_EDGES, enableCallback);
	gpioIRQ(CARD_CLK_PIN, GPIO_IRQ_MODE_FALLING_EDGE, clockCallback);
	////////////////////////////////////////////////////////////////////////////////////
	NVIC_EnableIRQ(PORTD_IRQn);
	//ESTO DEPENDE DE QUE PUERTO USEMOS

//	cardCallback = fun_callback;
}

uint8_t * cardGetPAN(void) {
	uint8_t i = 0;
	if ((enable == 0) && (error == false)) {
		while ((data[i + 1] != FS) && (i < PAN_LENGHT)) {
			pan[i] = data[i + 1] & LOW_NYBBLE_MASK;
			i++;
		}
		return &pan[0];
	} else {
		return 0;
	}

}
void enableCallback(void) {
	enable = !gpioRead(CARD_EN_PIN);
	if (enable) {
		bit_counter = 0;
		character = 0;
		char_counter = 0;
		error = false;
	} //seguro habrá que hacer algo si estoy enable tipo configuracion
//	else{
//
//	}

}
void clockCallback(void) { // seguro en algun momeno debería buscar los SS, FS y ES

	if (enable) {
		bool my_data = gpioRead(CARD_DATA_PIN);
		if ((bit_counter < CHAR_LENGHT) &&  (char_counter < DATA_LENGHT -1)) {
			character = character | (my_data << bit_counter++); //0 0 0 P b3 b2 b1 b0
		} else if (data[0] != SS){
			bit_counter--; // se encarga de buscar el start sentinel
			character = character >> 1;
		}
		else if(char_counter < DATA_LENGHT -1) {
			bit_counter = 0;
			data[char_counter++] = character;//stores the characters in the data buffer
			character = 0;
		}
		else if (data[char_counter-1] == ES) {
			char_counter = 0;
			if ((!checkParity())) {
				error = true;
				//si no da la paridad aca podes hace cosas
			}

		}
		else{
			error = true;// si todo salio mal  debe haber un error
		}

	}
}
bool checkParity(void) {
	uint8_t i = 0, j;
	uint8_t char_parity = false;
	uint8_t lrc_parity[CHAR_LENGHT] = { 0 };
	bool is_ok = true;
	for (i = 0; i < DATA_LENGHT; i++) {
		for (j = 0; j < CHAR_LENGHT; j++) {
			char_parity ^= ((data[i] & (1 << j)) >> j); //we test the parity of the chars
			lrc_parity[j] ^= ((data[i] & (1 << j)) >> j);
		}
		if (char_parity == 0) { //if it wasnt ODD parity.
			error = true;
			is_ok &= false;
			//////////////////////////////////
			//cout << "Parity error detected in character " << (int)i << endl;
			//////////////////////////////////
		}
		char_parity = false;
	}
	for (i = 0; i < CHAR_LENGHT - 1; i++)
		if (lrc_parity[i] != 1) {
			is_ok &= false;
			//////////////////////////////////
			//	cout << "Parity error in the character "<< (int) i << " of the LCR check" << endl;
			//////////////////////////////////
		}
	return is_ok;
}

