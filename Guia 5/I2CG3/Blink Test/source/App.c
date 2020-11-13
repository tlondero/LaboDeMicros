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
#include "header/Button.h"
//#include "header/uart.h"
#include "header/i2c.h"
#include "header/gpio.h"
#include "MK64F12.h"
#include "header/board.h"
#include "timer.h"
#include "hardware.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 *******************************************************************************
 ******************************************************************************/

/*******************************************************************************
 * INTERRUPCIONES
 ******************************************************************************/

/*******************************************************************************
 * BALIZA
 ******************************************************************************/

//static void update_baliza(int period);
void I2CRun(void);


void App_Init(void) {
	i2cInit(I2C_0);
}

/* Función que se llama constantemente en un ciclo infinito */


void App_Run(void) {
	I2CRun();
}

void I2CRun(void){

}

