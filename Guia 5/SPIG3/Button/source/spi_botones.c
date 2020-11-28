/*
 * spi_botones.c
 *
 *  Created on: 28 nov. 2020
 *      Author: Msrt
 */
#include "header/spi_botones.h"
void SPI_589_init(void){
	SPI_init();
}

static uint8_t source[1] = {0b00000000};//,20,20,20,10,10,10,10};
static uint8_t destination[1];

static action_589 a;

action_589 get_589_events(){
	spi_transaction(source, sizeof(source), destination);

	if(destination[0] & LEFT_589){ //left
		a = LEFT_589;
	}
	else if(destination[0] & DOWN_589){ //down
		a = DOWN_589;
	}
	else if(destination[0] & RIGHT_589){ //right
		a = RIGHT_589;
	}
	else if(destination[0] & ROTATE_589){ //rotate
		a = ROTATE_589;
	}
	//Flush
	spi_transaction(source, sizeof(source), destination);
	return a;
}
