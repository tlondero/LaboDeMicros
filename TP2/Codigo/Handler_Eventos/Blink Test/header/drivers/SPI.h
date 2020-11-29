/*
 * SPI.h
 *
 *  Created on: Nov 12, 2020
 *      Author: Acer
 */

#ifndef SPI_H_
#define SPI_H_

#include<stdint.h>
#include<stdbool.h>

void spi_init (void);
uint8_t spi_transaction(uint8_t * data_ptr, uint8_t size, uint8_t * recieve_ptr);

bool spi_data_sent(void);
uint8_t SPI_get_data(uint8_t * dataRecived);

#endif /* SPI_H_ */
