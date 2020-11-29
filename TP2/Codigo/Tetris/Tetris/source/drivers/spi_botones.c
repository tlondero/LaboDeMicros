/*
 * spi_botones.c
 *
 *  Created on: 28 nov. 2020
 *      Author: Msrt
 */
#include "header/drivers/SPI.h"
#include "header/drivers/spi_botones.h"

//Dummy MOSI
static uint8_t source[1] = {0b10000001}; //,20,20,20,10,10,10,10};
static uint8_t destination_button[1];

#define NO_EVENT_589 0
#define RESET_589 4
#define PAUSE_RESUME_589 8
#define LEFT_589 16
#define DOWN_589 32
#define RIGHT_589 64
#define ROTATE_589 128

static button_status button_589;

void SPI_589_init(void)
{
	spi_init();
}

void SPI_589_update_buttons(void)
{
	spi_transaction(source, sizeof(source), destination_button);
	spi_transaction(source, sizeof(source), destination_button);

	button_589.left = ((destination_button[0] & LEFT_589) != 0);
	button_589.right = ((destination_button[0] & RIGHT_589) != 0);
	button_589.down = ((destination_button[0] & DOWN_589) != 0);
	button_589.rotate = ((destination_button[0] & ROTATE_589) != 0);
	button_589.pause_resume = ((destination_button[0] & PAUSE_RESUME_589) != 0);
	button_589.reset = ((destination_button[0] & RESET_589) != 0);
}

//Returns an only read pointer to a struct holding status for all available buttons
status_pointer SPI_589_get_buttons_status(void)
{
	status_pointer p = &button_589;
	return p;
}
