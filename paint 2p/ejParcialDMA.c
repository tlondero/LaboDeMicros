status dma_status;
void initalize_transfer_driver(void){
	SPI_init();
	DMASPI_init();
}
int Start_Transfer(void){
	ConfigBuffer(buffer,interal_buffer);
	dma_status = BUSY;
	DMA0->TCD[0].CSR |= 1<<0;
	
}

status transfer_status(void){
	return dma_status
}


void ConfigBuffer(void){
	
}

////////////////////////DMA//////////////////////////////
void DMASPI_init(void){
    clockGating(DMA);
	MuxDMA(SPI);
	Nvic_config(DMA0);
	DMA0->TCD[0].SADDR = (uint32_t)(internal_buffstart);
	DMA0->TCD[0].DADDR = (uint32_t)(&(SPI0->PUSHR);
	
	DMA0->TCD[0].SOFF =0x04; // Source address offset of 2 bytes per transaction.
	DMA0->TCD[0].DOFF =0x00; // Destination address offset is 0. (Siempre al mismo lugar)
	DMA0->TCD[0].ATTR = DMA_ATTR_SSIZE(2) | DMA_ATTR_DSIZE(2);
	DMA0->TCD[0].NBYTES_MLNO = 0x04;
		
	DMA0->TCD[0].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(1024/2);	// div 2 	
	DMA0->TCD[0].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(1024/2);  // div 2

	DMA0->TCD[0].SLAST = -((4)*(1024/2));
	DMA0->TCD[0].DLAST_SGA = 0x00;
	DMA0->TCD[0].CSR = DMA_CSR_INTMAJOR_MASK;	//Enable Major Interrupt.
	/* Enable request signal for channel 0. */
	DMA0->ERQ = DMA_ERQ_ERQ0_MASK;
}

void DMA0_IRQHandler()
{
	/* Clear the interrupt flag. */
	DMA0->CINT |= 0;
	dma_status = DONE;
}
////////////////////////////////////////////////////////////



///////////////SPI|/////////////////////////////////////////////
void spi_init(void)
{
	init_spi_pin(PORTNUM2PIN(PD, 0));//PCS

	init_spi_pin(PORTNUM2PIN(PD, 1));
	init_spi_pin(PORTNUM2PIN(PD, 2));
	init_spi_pin(PORTNUM2PIN(PD, 3));

	//Clock gating
	SIM->SCGC6 |= SIM_SCGC6_SPI0(1);

	//Configure MCR
	SPI0->MCR = SPI_MCR_HALT_MASK | SPI_MCR_MSTR_MASK;
	SPI0->MCR = SPI_MCR_MSTR(1) | SPI_MCR_DCONF(0xb00) | SPI_MCR_MTFE(0) | SPI_MCR_PCSSE(0) | SPI_MCR_PCSIS(0) | SPI_MCR_MDIS(0) | SPI_MCR_DIS_TXF(0) | SPI_MCR_DIS_RXF(0);

	//Configure CTAR
	SPI0->CTAR[0] = SPI_CTAR_CPOL(0) | SPI_CTAR_CPHA(0) | SPI_CTAR_PBR(SPI_PBR) | SPI_CTAR_BR(SPI_BR) | SPI_CTAR_DBR(0) | SPI_CTAR_FMSZ(SPI_FRAME_SIZE - 1) | SPI_CTAR_ASC(SPI_ASC) | SPI_CTAR_PASC(SPI_PASC) | SPI_CTAR_CSSCK(SPI_CSSCK) | SPI_CTAR_PCSSCK(SPI_PCSSCK) | SPI_CTAR_PDT(0) | SPI_CTAR_DT(0);

	//Enable Master chip select
	SPI0->MCR |= SPI_MCR_PCSIS(0); //Select slave

	//Clock enable   //Stop communication  //Enable Master Mode
	SPI0->MCR |= SPI_MCR_MDIS(0) | SPI_MCR_HALT(0) | SPI_MCR_MSTR(1);
}

