/***************************************************************************//**
 @file     App.c
 @brief    Application functions
 @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


#include "header/Button.h"
#include "header/i2c.h"
#include "header/gpio.h"
#include "MK64F12.h"
#include "header/board.h"
#include "header/FXOS8700CQ.h"
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

uint8_t slave;
uint8_t reg;
uint8_t *data;
uint8_t size;
i2c_mode_t mode;
bool todoPiola;
bool todoPiola2;

static void cback(void){
	todoPiola2 = !todoPiola2;
}

void I2CInit_tb(void) {

	_mqx_ints_FXOS8700CQ_start();

	/*
	i2cInit(I2C_0);
	todoPiola = false;
	todoPiola2 = false;
	slave = 0x1d;
	reg = 0x0d;
	data = 0;
	size = 1;
	mode = I2C_READ;
	*/
}

/* Función que se llama constantemente en un ciclo infinito */

void I2CRun_tb(void) {
	ReadAccelMagnData(data);
	/*
	todoPiola = i2cTransaction(slave, reg, data, size, mode, cback);
	if (todoPiola) {
		uint8_t a = 0;
	}*/
}

