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

/*
 * Brief: initializes the spi module
 */
void spi_init (void);

/*
 * Brief: initializes a spi transaction where several bytes of data is transmitted and also read.
 * @param data_ptr: pointer to the data buffer to be transmitted
 * @param size: size of the data to be transmitted
 * @param recieve_ptr: buffer where data is to be stored. should be at least the same size or bigger than data buffer.
 */
uint8_t spi_transaction(uint8_t * data_ptr, uint8_t size, uint8_t * recieve_ptr);

#endif /* SPI_H_ */
