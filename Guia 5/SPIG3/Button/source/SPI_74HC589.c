/*
 * 74HC589_SPI.c
 *
 *  Created on: Nov 13, 2020
 *      Author: Acer
 */

#include "header/SPI_74HC589.h"

static uint8_t aux;
static uint8_t buffer;

void spi_74HC589_init(void){
	spi_init();
}

uint8_t spi_74HC589_read(void){
	spi_transaction(&aux, sizeof(aux), &buffer);
	return buffer;
}
