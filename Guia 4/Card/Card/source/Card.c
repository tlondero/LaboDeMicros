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
#define CARD_KINETIS_PIN ()
#define FS 0b01101
#define SS 0b01011
#define ES 0b11111
#define LOW_NYBBLE_MASK (0b00001111)
/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
static uint8_t pan[PAN_LENGHT];
static uint8_t data[DATA_LENGHT];//?
/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE DECLARATION
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE DEFINITION
 ******************************************************************************/
void cardSetCallback(void (*callback)(void)){
	gpioIRQ(CARD_KINETIS_PIN, GPIO_IRQ_MODE_FALLING_EDGE, callback);
}
void cardInitDriver(void){
	//???
}


uint8_t * cardGetPAN(void){
	uint8_t i=0;
	while((data[i+1] != FS) && (i< PAN_LENGHT)){
		pan[i]= data[i+1] & LOW_NYBBLE_MASK;
		i++;
	}
	return &pan[0];
}

