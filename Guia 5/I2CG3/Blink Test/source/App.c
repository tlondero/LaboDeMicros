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
uint8_t slave;
uint8_t reg;
uint8_t *data;
uint8_t size;
i2c_mode_t mode;
callbackptr cback;
bool todoPiola;

void I2CInit_tb(void) {
	i2cInit(I2C_0);
	todoPiola = false;
	slave = 0;
	reg = 0;
	data = 0;
	size = 8;
	mode = I2C_WRITE;
	cback = NULL;
}

/* Función que se llama constantemente en un ciclo infinito */

void I2CRun_tb(void) {
	todoPiola = i2cTransaction(slave, reg, data, size, mode, cback);
	if (todoPiola) {
		uint8_t a = 0;
	}
}

