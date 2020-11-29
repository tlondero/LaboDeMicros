/*
 * spi_botones.h
 *
 *  Created on: 28 nov. 2020
 *      Author: Msrt
 */

#ifndef SPI_BOTONES_H_
#define SPI_BOTONES_H_
#include "./SPI.h"
#include <stdint.h>

typedef struct
{
	bool left;
	bool right;
	bool down;
	bool rotate;
	bool pause_resume;
	bool reset;
} button_status;

typedef uint8_t action_589;
typedef button_status* _status_pointer;
typedef const button_status* status_pointer;


//PUBLIC API
void SPI_589_init(void);
void SPI_589_update_buttons(void);
status_pointer SPI_589_get_buttons_status(void);
#endif /* SPI_BOTONES_H_ */
