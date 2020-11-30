/*
 * DMA.h
 *
 *  Created on: Nov 14, 2020
 *      Author: Acer
 */

#ifndef DMA_H_
#define DMA_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief initializes the DMA module for the use of a matrix of ws2812b leds.
 * @param matrix_ptr: pointer to the matrix of FTM ChannelValues to transfer
 * @param matrix_size: size of the matrix buffer in bytes
 * @param cb_: function to call back on major loop completion
 */
void DMAInitWS2812b(uint16_t * matrix_ptr, uint32_t matrix_size, void* cb_);

#endif /* DMA_H_ */
