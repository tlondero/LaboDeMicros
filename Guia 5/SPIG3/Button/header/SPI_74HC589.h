/*
 * 74HC589_SPI.h
 *
 *  Created on: Nov 13, 2020
 *      Author: Acer
 */

#ifndef SPI_74HC589_H_
#define SPI_74HC589_H_

#include<stdint.h>
#include<stdbool.h>
#include "header/SPI.h"

void spi_74HC589_init(void);
uint8_t spi_74HC589_read(void);

#endif /* SPI_74HC589_H_ */
