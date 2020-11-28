/***************************************************************************/ /**
  @file     event_handler.h
  @brief
  @author   MAGT
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "../header/event_handlers/event_handler.h"
#include "../header/event_handlers/AccelerometerEvents.h"
#include "../header/event_handlers/uartPackHand.h"
#include "../header/event_handlers/paquetes.h"
#include "../header/drivers/uart.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION DECLARATION WITH LOCAL SCOPE
 ******************************************************************************/
void processEvents( package * PEvents);

/*******************************************************************************
 * FUNCTION DEFINITION WITH GLOBAL SCOPE
 ******************************************************************************/


void EvHandGetEvents(void){
	package data={0};
	accelerometerGetEvent(&data);//I2C events

	uartGetEvent(&data, U0);//Uart events

	//SPI events

	//ADC events

	processEvents(&data);
}



/*******************************************************************************
 * FUNCTION DEFINITION WITH LOCAL SCOPE
 ******************************************************************************/
void processEvents( package * PEvents){
	if(PEvents->reset){
		//reset function
	}
	else{
		if(PEvents->action.down){
			//Down function
		}
		if(PEvents->action.left){
			//Left function
		}
		if(PEvents->action.right){
			//Right function
		}
		if(PEvents->action.rotate){
			//rotate function
		}
		if(PEvents->birghtness.change){
			//brightness function
		}
		if(PEvents->fall_speed.change){
			//fall speed function
		}
		if(PEvents->piece.changed){
			//update piece property
		}
	}
}

