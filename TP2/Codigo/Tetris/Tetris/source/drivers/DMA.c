/*
 * DMA.c
 *
 *  Created on: Nov 14, 2020
 *      Author: Acer
 */

#include "header/drivers/DMA.h"
#include "header/drivers/FTM.h"
#include "hardware.h"

#define TRANSFER_SIZE 0x02*0x01

static void (*cb)(void);

void DMAInitWS2812b(uint16_t * matrix_ptr, uint32_t matrix_size, void* cb_){

	/* Store the callback provided. */
	cb = cb_;

	/* Enable the clock for the eDMA and the DMAMUX. */
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	/* Enable the eDMA channel 0 and set the FTM CH0 as the DMA request source. */
	DMAMUX->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(20);   // FTM0 CH0

	/* Clear all the pending events. */
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	/* Enable the DMA interrupts. */
	NVIC_EnableIRQ(DMA0_IRQn);

	/// ============= INIT TCD0 ===================//
	/* Set memory address for source and destination. */
	DMA0->TCD[0].SADDR = (uint32_t)(matrix_ptr);				   //List of Duties

	//DMA_TCD0_DADDR = (uint32_t)(destinationBuffer);
	DMA0->TCD[0].DADDR = (uint32_t)(&(FTM0->CONTROLS[0].CnV));  // To change FTM Duty


	/* Set an offset for source and destination address. */
	DMA0->TCD[0].SOFF =0x02; // Source address offset of 2 bytes per transaction.
	DMA0->TCD[0].DOFF =0x00; // Destination address offset is 0. (Siempre al mismo lugar)

	/* Set source and destination data transfer size to 16 bits (CnV is 2 bytes wide). */
	DMA0->TCD[0].ATTR = DMA_ATTR_SSIZE(1) | DMA_ATTR_DSIZE(1);

	/*Number of bytes to be transfered in each service request of the channel.*/
	DMA0->TCD[0].NBYTES_MLNO = (TRANSFER_SIZE);

	/* Current major iteration count. */
	DMA0->TCD[0].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(matrix_size/(TRANSFER_SIZE));	//(sizeof(sourceBuffer)/sizeof(sourceBuffer[0]))
	DMA0->TCD[0].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(matrix_size/(TRANSFER_SIZE));


	//DMA_TCD0_SLAST = 0x00;
	//DMA_TCD0_SLAST = -5*sizeof(uint16_t);
	//DMA_TCD0_SLAST = -((sizeof(sourceBuffer)/sizeof(sourceBuffer[0])*sizeof(uint16_t)));

	/* Autosize SLAST for Wrap Around. This value is added to SADD at the end of Major Loop */
	DMA0->TCD[0].SLAST = -matrix_size;


	/* DLASTSGA DLAST Scatter and Gatter */
	 DMA0->TCD[0].DLAST_SGA = 0x00;

	/* Setup control and status register. */

	DMA0->TCD[0].CSR = DMA_CSR_INTMAJOR_MASK;	//Enable Major Interrupt.


	/* Enable request signal for channel 0. */
	DMA0->ERQ = DMA_ERQ_ERQ0_MASK;

}

void DMA0_IRQHandler()
{
	/* Clear the interrupt flag. */
	DMA0->CINT |= 0;
	cb();
}

/* The red LED is toggled when an error occurs. */
void DMA_Error_IRQHandler()
{
	/* Clear the error interrupt flag.*/
	DMA0->CERR |= 0;

}

