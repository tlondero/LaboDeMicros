/***************************************************************************/ /**
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
#define FS 0b01101 //FS 0b10110 //
#define SS 0b01011
#define ES 0b11111
#define LOW_NYBBLE_MASK (0b00001111)
#define CARD_EN_PIN PORTNUM2PIN(PB, 11)	  //Amarillo
#define CARD_DATA_PIN PORTNUM2PIN(PC, 11) //Azul
#define CARD_CLK_PIN PORTNUM2PIN(PC, 10)  //Verde
#define DEVELOPMENT_MODE 1

#define LEN(array) sizeof(array) / sizeof(array[0])
/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
static uint8_t pan[PAN_LENGHT];
static uint8_t data[DATA_LENGHT]; //
static bool enable;
static bool error;

static bool go = false;		//Beging reading incoming binits
static bool es_rx = false;	//Flag que se enciende cuando se detecta ES
static bool lrc_rx = false; //Flag que se enciende cuando se detecta LRC
static bool ss_rx = false;		//Flag que se enciende cuando se dectecta SS
static bool card_avb = false; //Flag que se enciende cuando tenemos una tarjeta lista para procesar

//COUNTERS
static uint8_t bit_counter = 0;
static uint8_t character = 0;
static uint8_t char_counter = 0;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH FILE SCOPE DECLARATION
 ******************************************************************************/
//Callbacks
void cardCallback(void);
void enableCallback(void);
void clockCallback(void);
bool checkParity(void);
void flushData(void);
/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE DEFINITION
 ******************************************************************************/

void cardInitDriver(void (*fun_callback)(void))
{
	gpioMode(CARD_EN_PIN, INPUT_PULLUP);
	gpioMode(CARD_DATA_PIN, INPUT_PULLUP);
	gpioMode(CARD_CLK_PIN, INPUT_PULLUP);
	////////////////////////////////////////////////////////////////////////////////////
	gpioIRQ(CARD_EN_PIN, GPIO_IRQ_MODE_BOTH_EDGES, enableCallback);
	gpioIRQ(CARD_CLK_PIN, GPIO_IRQ_MODE_FALLING_EDGE, clockCallback);
	////////////////////////////////////////////////////////////////////////////////////
	NVIC_EnableIRQ(PORTC_IRQn);
	NVIC_EnableIRQ(PORTB_IRQn);

}
void flushData(void){
	int i = 0;
	for (i = 0; i < LEN(data); i++)
		data[i] = 0;
}


uint8_t *cardGetPAN(void)
{

	uint8_t i = 0;

	if ((card_avb==true) && (checkParity() == true)) //Si tenemos una tarjeta disponible y pasa
	{
		while ((data[i + 1] != FS) && (i < PAN_LENGHT))
		{
			pan[i] = data[i + 1] & LOW_NYBBLE_MASK;
			i++;
		}
		card_avb=false;
		flushData();
		return &pan[0];
	}
	else
	{
		return 0;
	}

}
static int ya_entre = 0;
void enableCallback(void)
{
	enable = !gpioRead(CARD_EN_PIN);

	if (ya_entre == 0)
	{
		flushData();
	}
	ya_entre++;
	if (ya_entre == 2)
	{
		ya_entre = 0;
	}


	bit_counter = 0;
	character = 0;
	char_counter = 0;
	go = false;	   //Data stream start flag
	es_rx = false; //
	lrc_rx = false;
	ss_rx = false;
	error = false;
}




void clockCallback(void)
{
	bool my_data = !gpioRead(CARD_DATA_PIN); // Read incoming bit_counter

	//Begin reading data stream.
	if (my_data)
	{
		go = true;
	}
	//Read new character
	if ((bit_counter < CHAR_LENGHT) && (char_counter < DATA_LENGHT) && (lrc_rx == false) && (go == true))
	{
		character = character | (my_data << bit_counter); //0 0 0 P b3 b2 b1 b0
	}

	//Si ya terminamos de contar
	if (bit_counter == (CHAR_LENGHT - 1) && (lrc_rx == false) && (go == true))
	{
		if ((character == SS) && (ss_rx == false))
		{
			ss_rx = true; //START SENTINEL RECEIVED
		}
		else if ((character == ES) && (ss_rx == true))
		{
			es_rx = true; //END SENTINEL RECEIVED
		}
		else if (es_rx == true)
		{
			lrc_rx = true;
			card_avb = true;
		}
		else{
			card_avb = false;
		}
		data[char_counter++] = character; //stores the characters in the data buffer
		character = 0;
		bit_counter = 0;
	}
	//Sino terminamos de contar
	else if (go == true && lrc_rx == false)
	{
		bit_counter++;
	}
}

bool checkParity(void)
{
	uint8_t i = 0, j;
	uint8_t char_parity = false;
	uint8_t lrc_parity[CHAR_LENGHT] = {0};
	bool is_ok = true;
	for (i = 0; i < char_counter; i++)
	{
		for (j = 0; j < CHAR_LENGHT; j++)
		{
			char_parity ^= ((data[i] & (1 << j)) >> j); //we test the parity of the chars
			lrc_parity[j] ^= ((data[i] & (1 << j)) >> j);
		}
		if (char_parity == 0)
		{ //if it wasnt ODD parity.
			error = true;
			is_ok = false;
		}
		char_parity = 0;
	}
	for (i = 0; i < CHAR_LENGHT - 1; i++)
	{
		if (lrc_parity[i] == 1)
		{
			is_ok = false;
			error = true;
		}
	}
	return is_ok;
}
