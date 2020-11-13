/*
 * SPI.c
 *
 *  Created on: Nov 12, 2020
 *      Author: Acer
 */

#include "header/board.h"
#include "MK64F12.h"
#include <stdio.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define SPI_MUX		2
#define SPI_INPUT	1

#define SPI_FRAME_SIZE 8
// 200 Kbauds == SysClock/BR = 1Meg/5 = 200Khz
#define SPI_BR 5
#define SPI_ASC 2
#define SPI_PASC 2
#define SPI_PBR 1
#define SPI_CSSCK 2
#define SPI_PCSSCK 1

/*******************************************************************************
 * VARIABLES WITH LOCAL SCOPE USING STATIC
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS, STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef uint8_t pin_t;
static uint32_t simMasks[] = {SIM_SCGC5_PORTA_MASK, SIM_SCGC5_PORTB_MASK, SIM_SCGC5_PORTC_MASK, SIM_SCGC5_PORTD_MASK, SIM_SCGC5_PORTE_MASK};
static PORT_Type *portPtrs[] = PORT_BASE_PTRS;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH LOCAL SCOPE
 ******************************************************************************/

void init_spi_pin(pin_t pin)
{
    SIM->SCGC5 |= simMasks[PIN2PORT(pin)];
    PORT_Type *port = portPtrs[PIN2PORT(pin)];
    port->PCR[PIN2NUM(pin)] = 0x00;
    port->PCR[PIN2NUM(pin)] = PORT_PCR_MUX(SPI_MUX)
    						|PORT_PCR_DSE(1);
}

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void spi_init (void){
	init_spi_pin(PORTNUM2PIN(PD, 0));
	init_spi_pin(PORTNUM2PIN(PD, 1));
	init_spi_pin(PORTNUM2PIN(PD, 2));
	init_spi_pin(PORTNUM2PIN(PD, 3));

	//Clock gating
	SIM->SCGC6 |= SIM_SCGC6_SPI0(1);

	//Configure MCR
	SPI0->MCR= SPI_MCR_HALT_MASK | SPI_MCR_MSTR_MASK;
	SPI0->MCR = SPI_MCR_MSTR(1) \
			| SPI_MCR_DCONF(0xb00) \
			| SPI_MCR_MTFE(0) \
			| SPI_MCR_PCSSE(0) \
			| SPI_MCR_PCSIS(0) \
			| SPI_MCR_MDIS(0) \
			| SPI_MCR_DIS_TXF(0) \
			| SPI_MCR_DIS_RXF(0);

	//Configure CTAR
	SPI0->CTAR[0] = SPI_CTAR_CPOL(0) \
			 | SPI_CTAR_CPHA(0) \
			 | SPI_CTAR_PBR(SPI_PBR) \
			 | SPI_CTAR_BR(SPI_BR) \
			 | SPI_CTAR_DBR(0) \
			 | SPI_CTAR_FMSZ(SPI_FRAME_SIZE-1) \
			 | SPI_CTAR_ASC(SPI_ASC) \
			 | SPI_CTAR_PASC(SPI_PASC) \
			 | SPI_CTAR_CSSCK(SPI_CSSCK) \
			 | SPI_CTAR_PCSSCK(SPI_PCSSCK) \
			 | SPI_CTAR_PDT(0) \
			 | SPI_CTAR_DT(0);

	//Enable Master chip select
	SPI0->MCR |= SPI_MCR_PCSIS(1); //Select slave

				//Clock enable   //Stop communication  //Enable Master Mode
	SPI0->MCR |= SPI_MCR_MDIS(0) |SPI_MCR_HALT(0) | SPI_MCR_MSTR(1);

}

uint8_t spi_transaction(uint8_t * data_ptr, uint8_t len, uint8_t * recieve_ptr){
	uint32_t pushr_data = 0;
	uint8_t data_i = 0;
	uint8_t send_i = 0;

	while(send_i < len){
		//Datos intermedios
		if(send_i < len - 1){
			pushr_data = SPI_PUSHR_PCS(1) \
						|SPI_PUSHR_CONT(1) \
						|SPI_PUSHR_TXDATA(data_ptr[send_i]);
		}
		//Ultimo dato
		else{
			pushr_data = SPI_PUSHR_PCS(1) \
						|SPI_PUSHR_CONT(0) \
						|SPI_PUSHR_EOQ(1) \
						|SPI_PUSHR_TXDATA(data_ptr[send_i]);
		}

		SPI0->MCR |= SPI_MCR_HALT(1);	//Halt transfers
		SPI0->SR |= SPI_SR_TCF(1); 		//TCF reset
		SPI0->PUSHR = pushr_data;		//Write to PUSHR
		SPI0->MCR &= ~SPI_MCR_HALT(1);	//Start transfers

		while(!(SPI0->SR & SPI_SR_TCF_MASK)){
			//Wait until transfer is done
		}
		SPI0->SR |= SPI_SR_TCF(1); 		//TCF reset

		while((SPI0->SR & SPI_SR_RXCTR_MASK)>>SPI_SR_RXCTR_SHIFT){	//RXCTR counts valid entries on RX FIFO
			if(recieve_ptr){
				recieve_ptr[data_i]=SPI0->POPR;	//Store recieved data on provided buffer
			}
			else{
				SPI0->POPR;	//Discard recieved data
			}
			data_i++;
		}
		send_i++;
	}
	return (data_i != 0);
}
