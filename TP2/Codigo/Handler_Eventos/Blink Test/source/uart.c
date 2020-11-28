/***************************************************************************//**
 @file     uart.c
 @brief    Uart driver for MK64
 @author   MAGT
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "header/uart.h"
#include "header/gpio.h"
#include <stdbool.h>
#include "hardware.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
// baud rate related defines
#define MAX_BAUD_RATE ((uint32_t)0xFFFF)
#define MIN_BAUD_RATE 0x0000
#define BAUD_RATE_DEFAULT 9600
// Initial config for port
#define DISABLE_MODE 0 //disable port interrupts for uart's pins
#define UART_MUX 3

#define RX_BUFFER_LEN 4000//Buffers len RX

#define TX_BUFFER_LEN 4000//Buffers len TX


#define EMPTY 0
/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

//Selected mode of each uart. blocking is the default
static uint8_t uartMode[UART_CANT_IDS] = { BLOCKING, BLOCKING, BLOCKING,BLOCKING, BLOCKING };

static const UART_Type * UART_POINTERS[] = UART_BASE_PTRS;

//variables de RX
static uint8_t UART_RX_PORTS[] = { PB, PC, PD, PC, PE };
static uint8_t UART_RX_BITS[] = { 16, 3, 2, 16, 24 };//Port and bit of the RX UART modules

static char RX_buffers[UART_CANT_IDS][RX_BUFFER_LEN]; //buffers for the uarts modules

static int latest_ch_rec_cnt_RX[UART_CANT_IDS] = { EMPTY, EMPTY, EMPTY, EMPTY,EMPTY };

static bool flags_reading[UART_CANT_IDS] = { false, false, false, false, false }; //Flag for the reading
static uint8_t counters_RX_failed[UART_CANT_IDS] = { 0, 0, 0, 0, 0 };


//variables de TX
static uint8_t UART_TX_PORTS[] = { PB, PC, PD, PC, PE };
static uint8_t UART_TX_BITS[] = { 17, 4, 3, 17, 25 };//Port and bit of the TX UART modules

static char TX_buffers[UART_CANT_IDS][TX_BUFFER_LEN];
static uint32_t latest_ch_sent_cnt_TX[UART_CANT_IDS] = { 0, 0, 0, 0, 0 };
static uint32_t latest_ch_rec_cnt_TX[UART_CANT_IDS] = { 0, 0, 0, 0, 0 };
static uint32_t lens_TX_buffer[UART_CANT_IDS] = { 0, 0, 0, 0, 0 };

//interruptions
uint8_t uartIRQs_TX_RX[] = UART_RX_TX_IRQS;
uint8_t uartIRQs_ERR[] = UART_ERR_IRQS;
/*******************************************************************************
 * FUNCTION PROTOTYPES WITH FILE SCOPE DECLARATION
 ******************************************************************************/
void UART_setBaudRate(UART_Type * uart_p, uint32_t baudRate);

void UARTX_RX_TX_IRQHandler(uint8_t id);
void UART0_RX_TX_IRQHandler(void);
void UART1_RX_TX_IRQHandler(void);
void UART2_RX_TX_IRQHandler(void);
void UART3_RX_TX_IRQHandler(void);
void UART4_RX_TX_IRQHandler(void);

void UARTX_ERR_IRQHandler(uint8_t id);
void UART0_ERR_IRQHandler(void);
void UART1_ERR_IRQHandler(void);
void UART2_ERR_IRQHandler(void);
void UART3_ERR_IRQHandler(void);
void UART4_ERR_IRQHandler(void);


void copyTXmsg(uint8_t id, const char * msg, uint32_t len);


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE DEFINITION
 *******************************************************************************/
void uartInit(uint8_t id, uart_cfg_t config) {
	SIM_Type * sim = SIM; //
	UART_Type * UART_POINTERS[] = UART_BASE_PTRS;
	UART_Type * uart_p;
	PORT_Type * PORT_POINTERS[] = PORT_BASE_PTRS;

	uint8_t portRX = UART_RX_PORTS[id]; //Port and bit of the RX and TX
	uint8_t num_bit_RX = UART_RX_BITS[id];
	uint8_t portTX = UART_TX_PORTS[id];
	uint8_t num_bit_TX = UART_TX_BITS[id];

	//Clock gating
	if ((id >= 0) && (id < UART_CANT_IDS)) //Checks valid ID
			{
		uart_p = UART_POINTERS[id];

		switch (id) {
		case U0:
			sim->SCGC4 |= SIM_SCGC4_UART0_MASK;
			break;
		case U1:
			sim->SCGC4 |= SIM_SCGC4_UART1_MASK;
			break;
		case U2:
			sim->SCGC4 |= SIM_SCGC4_UART2_MASK;
			break;
		case U3:
			sim->SCGC4 |= SIM_SCGC4_UART3_MASK;
			break;
		case U4:
			sim->SCGC1 |= SIM_SCGC1_UART4_MASK;
			break;
		}

		uart_p->C2 &= (~(UART_C2_TE_MASK | UART_C2_RE_MASK));
		//disable tx and rx

		UART_setBaudRate(uart_p, config.baudRate);
		//sets baudrate
		switch (config.parity) {
		case ODD_PARITY_UART:
			uart_p->C1 |= UART_C1_PE_MASK;
			uart_p->C1 |= UART_C1_PT_MASK;
			uart_p->C1 |= UART_C1_M_MASK; //parity
			break;
		case EVEN_PARITY_UART:
			uart_p->C1 |= UART_C1_PE_MASK;
			uart_p->C1 &= (~UART_C1_PT_MASK);
			uart_p->C1 |= UART_C1_M_MASK;
			break;
		case NO_PARITY_UART:
			uart_p->C1 &= ~UART_C1_M_MASK;
			uart_p->C1 &= (~UART_C1_PE_MASK);
			break;
		}

		uart_p->BDH &= ~UART_BDH_SBNS_MASK;
		/*
		 * Stop Bit Number Select
		 SBNS selects the number of stop bits present in a data frame. This field valid for all 8, 9 and 10 bit data
		 formats available. This field is not valid when C7816[ISO7816E] is enabled.
		 0 Data frame consists of a single stop bit. <--- selected
		 1 Data frame consists of two stop bits.*/

		(PORT_POINTERS[portRX]->PCR)[num_bit_RX] &= ~PORT_PCR_MUX_MASK;
		(PORT_POINTERS[portRX]->PCR)[num_bit_RX] |= PORT_PCR_MUX(UART_MUX); //sets mux

		(PORT_POINTERS[portTX]->PCR)[num_bit_TX] &= ~PORT_PCR_MUX_MASK;
		(PORT_POINTERS[portTX]->PCR)[num_bit_TX] |= PORT_PCR_MUX(UART_MUX);

		(PORT_POINTERS[portRX]->PCR)[num_bit_RX] &= ~PORT_PCR_IRQC_MASK;
		(PORT_POINTERS[portRX]->PCR)[num_bit_RX] |= PORT_PCR_IRQC(DISABLE_MODE); //disable interrupt

		(PORT_POINTERS[portTX]->PCR)[num_bit_TX] &= ~PORT_PCR_IRQC_MASK;
		(PORT_POINTERS[portTX]->PCR)[num_bit_TX] |= PORT_PCR_IRQC(DISABLE_MODE);

		if (config.mode != BLOCKING) {
			NVIC_EnableIRQ(uartIRQs_TX_RX[id]); //enable interrupt
			NVIC_EnableIRQ(uartIRQs_ERR[id]);
			uart_p->C2 |= UART_C2_RIE_MASK;
			/*
			 * Receiver Full Interrupt or DMA Transfer Enable
			 Enables S1[RDRF] to generate interrupt requests or DMA transfer requests, based on the state of
			 C5[RDMAS].
			 0 RDRF interrupt and DMA transfer requests disabled.
			 1 RDRF interrupt or DMA transfer requests enabled*/
		}
		uart_p->C2 |= (UART_C2_RE_MASK);			//activation of RX mode
	}
	uartMode[id] = config.mode;
}

bool uartIsRxMsg(uint8_t id) {
	UART_Type * uart_p;
	uart_p = UART_POINTERS[id];
	bool ret = false;
	if ((uartMode[id] == NON_BLOCKING)) {
		if (latest_ch_rec_cnt_RX[id] == EMPTY) {
			ret = false; //checks if there is new info
		} else {
			ret = true;
		}
	} else if (uartMode[id] == BLOCKING) {
		ret = uart_p->S1 & UART_S1_RDRF_MASK; //waits for the RDRF to go up
	}
	return ret;
}

uint32_t uartGetRxMsgLength(uint8_t id) {
	return (latest_ch_rec_cnt_RX[id] + 1); //
}

uint32_t uartReadMsg(uint8_t id, char* msg, uint32_t len) {
	uint32_t ret = 0;
	UART_Type * uart_p = UART_POINTERS[id];
	uint32_t i = 0, j;
	if ((id >= 0) && (id < UART_CANT_IDS)) {
		if (uartMode[id] == BLOCKING) {
			while ((uart_p->S1 & UART_S1_RDRF_MASK) && (i < len)) { //if there is new info
				msg[i] = uart_p->D; //puts it in the msg buffer
				i++;
			}
			ret = i;
		} else if (((uartMode[id] == NON_BLOCKING))
				&& (latest_ch_rec_cnt_RX[id] != EMPTY)) {
			flags_reading[id] = true; //to not be interrupted while doing this
			for (i = 0; (i < len) && (i <= latest_ch_rec_cnt_RX[id]); i++) {
				msg[i] = RX_buffers[id][i]; // reads the buffer
			}
			//shift the data "len" times
			for (j = 0; j < (latest_ch_rec_cnt_RX[id] + 1 - len); j++) {
				RX_buffers[id][j] = RX_buffers[id][len + j];
			}
			latest_ch_rec_cnt_RX[id] = (j == 0 ? EMPTY : (j - 1)); //corrects the counter
			flags_reading[id] = false; //finished
			ret = i;
		}
	}
	return ret;
}

uint32_t uartWriteMsg(uint8_t id, const char* msg, uint32_t len) {
	UART_Type * uart_p = UART_POINTERS[id];
	uint32_t i = 0;
	uint8_t len_TX = 0;
	bool uart_working = false;
	if ((id >= 0) && (id < UART_CANT_IDS)) {
		if (uartMode[id] == BLOCKING) {
			while (i < len) {
				if ((uart_p->S1 & UART_S1_TDRE_MASK)) { //If i can transmit, then transmit everything
					uart_p->D = msg[i];
					i++;
				}
			}
			len_TX = len;
		} else if ((uartMode[id] == NON_BLOCKING)) {
			if (lens_TX_buffer[id] == 0) { //if i wasnt transmitting anythig i gotta start it
				uart_working = true;
			}
			if (len < (TX_BUFFER_LEN - lens_TX_buffer[id]))
				len_TX = len; //if len fits in the buffer
			else
				len_TX = 0;
			if (len_TX == len) {
				copyTXmsg(id, msg, len_TX); //send msg to the buffer
				if ((uart_working || ~(uart_p->C2 & UART_C2_TCIE_MASK))
						&& (len_TX > 0)) //if i wasnt using the uart or i just finished using it
						{
					uart_p->C2 |= UART_C2_TE_MASK;
					uart_p->C2 |= UART_C2_TIE_MASK; //finished transmission interrupt
				}
			} else {
				len_TX = 0;
			}
		}
	}
	return len_TX;
}

uint32_t uartIsTxMsgComplete(uint8_t id) {
	UART_Type * uart_p;
	uint8_t ret = 0;
	if ((id >= 0) && (id < UART_CANT_IDS)) {
		uart_p = UART_POINTERS[id];
		ret = uart_p->C2 & UART_C2_TCIE_MASK; //Transmission Complete Interrupt or DMA Transfer Enable
	} //if i finished the tx this will be true
	return ret;
}


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH FILE SCOPE DEFINITION
 *******************************************************************************/
void copyTXmsg(uint8_t id, const char * msg, uint32_t len) {
	uint32_t i;
	for (i = 0; i < len; i++) {
		TX_buffers[id][latest_ch_rec_cnt_TX[id]] = msg[i];
		if (latest_ch_rec_cnt_TX[id] < TX_BUFFER_LEN - 1) {
			latest_ch_rec_cnt_TX[id]++; //if im not finished advance one position
		} else
			latest_ch_rec_cnt_TX[id] = 0; //im finished
		(lens_TX_buffer[id])++;
	}
}

void UARTX_RX_TX_IRQHandler(uint8_t id) {

	UART_Type * uart_p = UART_POINTERS[id];

	if (uart_p->S1 & UART_S1_RDRF_MASK) { //recibi data
		if (!flags_reading[id]) {
			if (latest_ch_rec_cnt_RX[id] < RX_BUFFER_LEN - 1) {

				RX_buffers[id][(latest_ch_rec_cnt_RX[id])] = uart_p->D;
				(latest_ch_rec_cnt_RX[id])++; //Points to the position of hte latest ch entered
				counters_RX_failed[id] = 0;
			} else {(counters_RX_failed[id])++;}

		}
	} else if (uart_p->S1 & UART_S1_TDRE_MASK) { //Transmit Data Register Empty Flag
		if (lens_TX_buffer[id] > 0) { //if theres stuff in the buffer
			uart_p->D = TX_buffers[id][latest_ch_sent_cnt_TX[id]];
			if (latest_ch_sent_cnt_TX[id] < TX_BUFFER_LEN - 1) {(latest_ch_sent_cnt_TX[id])++;}
			else {latest_ch_sent_cnt_TX[id] = 0;}
			lens_TX_buffer[id]--;
		} else {
			uart_p->C2 = (uart_p->C2 & (~UART_C2_TE_MASK));
			uart_p->C2 = uart_p->C2 & (~UART_C2_TIE_MASK);
		}
	}
}

void UARTX_ERR_IRQHandler(uint8_t id) {
//hacer algo cuando haya un error
}

void UART0_RX_TX_IRQHandler(void) {
	UARTX_RX_TX_IRQHandler(U0);
}
void UART0_ERR_IRQHandler(void) {
	UARTX_ERR_IRQHandler(U0);
}
void UART1_RX_TX_IRQHandler(void) {
	UARTX_RX_TX_IRQHandler(U1);
}
void UART1_ERR_IRQHandler(void) {
	UARTX_ERR_IRQHandler(U1);
}
void UART2_RX_TX_IRQHandler(void) {
	UARTX_RX_TX_IRQHandler(U2);
}
void UART2_ERR_IRQHandler(void) {
	UARTX_ERR_IRQHandler(U2);
}
void UART3_RX_TX_IRQHandler(void) {
	UARTX_RX_TX_IRQHandler(U3);
}
void UART3_ERR_IRQHandler(void) {
	UARTX_ERR_IRQHandler(U3);
}
void UART4_RX_TX_IRQHandler(void) {
	UARTX_RX_TX_IRQHandler(U4);
}
void UART4_ERR_IRQHandler(void) {
	UARTX_ERR_IRQHandler(U4);
}

void UART_setBaudRate(UART_Type * uart_p, uint32_t baudRate) {
	uint8_t brfa;
	uint16_t sbr;
	uint32_t clock;

	clock = ((uart_p == UART0) || (uart_p == UART1)) ?
			(__CORE_CLOCK__) : (__CORE_CLOCK__ >> 1); //CORE_CLOCK o BUS_CLOCK

	if ((baudRate <= MIN_BAUD_RATE) || (baudRate >= MAX_BAUD_RATE)) {
		baudRate = BAUD_RATE_DEFAULT;    //default baudrate
	}

	sbr = clock / (baudRate << 4);
	brfa = ((clock << 1) / baudRate) - (sbr << 5);

	uart_p->BDH = UART_BDH_SBR((sbr >> 8)); //BDH
	uart_p->BDL = UART_BDL_SBR(sbr);  //BDL
	uart_p->C4 = (uart_p->C4 & (~UART_C4_BRFA_MASK)) | UART_C4_BRFA(brfa);

}

