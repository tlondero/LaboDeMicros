#include "hardware.h"
#include "MK64F12.h"
#include "gpio.h"
#include <stdint.h>
#include <stdbool.h>

/*************
 * SPI DRIVER
 *************/

/*
Main Services
 -Init SPI port. There are 6 SPI ports in total
 -Set port Baud Rate
 -Set SCK polarity and phase
 -Choose ports frame width (from 4 to 16 bits)
 -Send messages to any Slave
 -Subscribe to receive messages from any of the slaves if enabled
*/
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define SPI_BAD 255

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef uint8_t spi_id_t;
typedef const char *message_ptr;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
spi_id_t spi_init(pin_t sck, pin_t miso, pin_t mosi, pin_t pcs);

void spi_send_message(spi_id_t id, message_ptr message);

message_ptr spi_read_message(spi_id_t id);
