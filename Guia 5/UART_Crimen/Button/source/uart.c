/*
 * uart.c
 *
 *  Created on: Oct 30, 2020
 *      Author: Guido
 */

#include "header/uart.h"
//#include "pinsHandler.h"
#include "header/gpio.h"
#include <stdbool.h>
#include "hardware.h"

#define MAX_BAUD_RATE ((uint32_t)0xFFFF)
#define MIN_BAUD_RATE 0x0000
#define BAUD_RATE_DEFAULT 9600

#define MAX_WATER_MARK 128
#define TX_WATER_MARK_DEFAULT 2
#define RX_WATER_MARK_DEFAULT 5
#define NBIT_DEFAULT 8

#define DISABLE_MODE 0 //disable port interrupts for uart's pins

#define UART_MUX 3

#define RX_BUFFER_LEN 8000
#define TX_BUFFER_LEN 2000

#define EMPTY -1


static uint8_t uartRXpins[] = { PORTNUM2PIN(PB, 16), PORTNUM2PIN(PC, 3),
		PORTNUM2PIN(PD, 2), PORTNUM2PIN(PC, 16), PORTNUM2PIN(PE, 24) };

//TX_0				//TX_1				//TX_2				//TX_3				//TX_4
static uint8_t uartTXpins[] = { PORTNUM2PIN(PB, 17), PORTNUM2PIN(PC, 4),
		PORTNUM2PIN(PD, 3), PORTNUM2PIN(PC, 17), PORTNUM2PIN(PE, 25) };

static uint8_t uartMode[UART_CANT_IDS] = { BLOCKING, BLOCKING, BLOCKING,
		BLOCKING, BLOCKING };

static const UART_Type * UART_POINTERS[] = UART_BASE_PTRS;

//variables de RX
static char RX_buffers[UART_CANT_IDS][RX_BUFFER_LEN];
static int markers_RX_buffer[UART_CANT_IDS] =
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY };
static _Bool flags_reading[UART_CANT_IDS] = { false, false, false, false, false };
static uint8_t counters_RX_failed[UART_CANT_IDS] = { 0, 0, 0, 0, 0 };

//variables de TX
static char TX_buffers[UART_CANT_IDS][TX_BUFFER_LEN];
static uint8_t out_Markers_TX_buffer[UART_CANT_IDS] = { 0, 0, 0, 0, 0 };
static uint8_t in_markers_TX_buffer[UART_CANT_IDS] = { 0, 0, 0, 0, 0 };
static uint8_t lens_TX_buffer[UART_CANT_IDS] = { 0, 0, 0, 0, 0 };


//
uint8_t uartIRQs_TX_RX[] = UART_RX_TX_IRQS;
uint8_t uartIRQs_ERR[] = UART_ERR_IRQS;
uint8_t uartIRQs_LON[] = UART_LON_IRQS;


void UART_setBaudRate(UART_Type * p2uart, uint32_t baudRate);



void UARTX_LON_IRQHandler(uint8_t id);
void UARTX_RX_TX_IRQHandler(uint8_t id);
void UARTX_ERR_IRQHandler(uint8_t id);

void UART0_LON_IRQHandler(void);
void UART0_RX_TX_IRQHandler(void);
void UART0_ERR_IRQHandler(void);
void UART1_RX_TX_IRQHandler(void);
void UART1_ERR_IRQHandler(void);
void UART2_RX_TX_IRQHandler(void);
void UART2_ERR_IRQHandler(void);
void UART3_RX_TX_IRQHandler(void);
void UART3_ERR_IRQHandler(void);
void UART4_RX_TX_IRQHandler(void);
void UART4_ERR_IRQHandler(void);

void copyTXmsg(uint8_t id, const char * msg, uint8_t cant);


void uartInit(uint8_t id, uart_cfg_t config) {
	SIM_Type * sim = SIM;
	UART_Type * UART_POINTERS[] = UART_BASE_PTRS;
	UART_Type * p2uart;
	PORT_Type * PORT_POINTERS[] = PORT_BASE_PTRS;
	uint8_t portRX,
	portTX, num_pin_RX, num_pin_TX;

	if ((id >= 0) && (id < UART_CANT_IDS)) //solo se hace algo si el id es válido
			{
		p2uart = UART_POINTERS[id];

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

		p2uart->C2 &= (~(UART_C2_TE_MASK | UART_C2_RE_MASK)); //para configurar correctamente deshabilito la transmision y la recepcion

		UART_setBaudRate(p2uart, config.baudRate);
		switch (config.parity) {
		case ODD_PARITY_UART:
			p2uart->C1 |= UART_C1_PE_MASK;
			p2uart->C1 |= UART_C1_PT_MASK;
			p2uart->C1 |= UART_C1_M_MASK;
			break;
		case EVEN_PARITY_UART:
			p2uart->C1 |= UART_C1_PE_MASK;
			p2uart->C1 &= (~UART_C1_PT_MASK);
			p2uart->C1 |= UART_C1_M_MASK;
			break;
		case NO_PARITY_UART:
			p2uart->C1 &= ~UART_C1_M_MASK;
			p2uart->C1 &= (~UART_C1_PE_MASK);
			break;
		}

		p2uart->BDH &= ~UART_BDH_SBNS_MASK;

		portRX = PIN2PORT(uartRXpins[id]);
		num_pin_RX = PIN2NUM(uartRXpins[id]);
		portTX = PIN2PORT(uartTXpins[id]);
		num_pin_TX = PIN2NUM(uartTXpins[id]);

		(PORT_POINTERS[portRX]->PCR)[num_pin_RX] &= ~PORT_PCR_MUX_MASK;
		(PORT_POINTERS[portRX]->PCR)[num_pin_RX] |= PORT_PCR_MUX(UART_MUX);

		(PORT_POINTERS[portTX]->PCR)[num_pin_TX] &= ~PORT_PCR_MUX_MASK;
		(PORT_POINTERS[portTX]->PCR)[num_pin_TX] |= PORT_PCR_MUX(UART_MUX);


		(PORT_POINTERS[portRX]->PCR)[num_pin_RX] &= ~PORT_PCR_IRQC_MASK;
		(PORT_POINTERS[portRX]->PCR)[num_pin_RX] |= PORT_PCR_IRQC(DISABLE_MODE);
		(PORT_POINTERS[portTX]->PCR)[num_pin_TX] &= ~PORT_PCR_IRQC_MASK;
		(PORT_POINTERS[portTX]->PCR)[num_pin_TX] |= PORT_PCR_IRQC(DISABLE_MODE);


		uartMode[id] = config.mode; //se guarda el modo (BLOCKING, NON_BLOCKING_SIMPLE, FIFO, etc).

		if (config.mode != BLOCKING) //configuración para version no bloqueante
			{
			NVIC_EnableIRQ(uartIRQs_TX_RX[id]);
			NVIC_EnableIRQ(uartIRQs_ERR[id]);
			if (id == U0) {
				NVIC_EnableIRQ(uartIRQs_LON[id]);
			}
			p2uart->C2 |= UART_C2_RIE_MASK;
		}
		p2uart->C2 |= (UART_C2_RE_MASK);

	}
}

bool uartIsRxMsg(uint8_t id) {
	UART_Type * p2uart;
	p2uart = UART_POINTERS[id];
	bool ret = false;
	if ((uartMode[id] == NON_BLOCKING)) {
		if (markers_RX_buffer[id] == EMPTY) {
			ret = false;
		} else {
			ret = true;
		}
	} else if (uartMode[id] == BLOCKING) {
		ret = p2uart->S1 & UART_S1_RDRF_MASK;
	}

	return ret;
}

uint8_t uartGetRxMsgLength(uint8_t id) {
	return (markers_RX_buffer[id] + 1);
}

uint8_t uartReadMsg(uint8_t id, char* msg, uint8_t cant) {
	uint8_t ret = 0;
	UART_Type * p2uart;
	uint32_t i = 0,
	j;
	if ((id >= 0) && (id < UART_CANT_IDS)) //solo se hace algo si el id es válido
			{
		p2uart = UART_POINTERS[id];
		if (uartMode[id] == BLOCKING) //bloquea hasta leer "cant" numero de chars.
				{
			while ((p2uart->S1 & UART_S1_RDRF_MASK) && (i < cant)) //tengo algo que leer?
			{
				msg[i] = p2uart->D;
				i++;
			}
			ret = i;
		} else if (((uartMode[id] == NON_BLOCKING))
				&& (markers_RX_buffer[id] != EMPTY)) //lee todo lo del buffer, siempre y cuando halla algo para leer
				{
			flags_reading[id] = true; //aviso que voy a leer asi no me pisan el buffer mientras lo leo
			for (i = 0; (i < cant) && (i <= markers_RX_buffer[id]); i++) {
				msg[i] = RX_buffers[id][i]; //hago pop de la cantidad de char. si no llego a "cant" de chars en el buffer, popeo todo lo que tengo
			}
			//actualizo el buffer (lo shifteo) y el marker si es necesario
			for (j = 0; j < (markers_RX_buffer[id] + 1 - cant); j++) //solo entro al for cuando cant es menor a (marker + 1)
					{
				RX_buffers[id][j] = RX_buffers[id][cant + j]; //muevo al principio del buffer los datos que quedaron sin popear
			}
			markers_RX_buffer[id] = (j == 0 ? EMPTY : (j - 1)); //corrijo el marker que apunta al último char
			flags_reading[id] = false; //aviso que termine de leer asi escriben el buffer tranquilos
			ret = i;
		}
	}
	return ret;
}

uint8_t uartWriteMsg(uint8_t id, const char* msg, uint8_t cant) {
	UART_Type * UART_POINTERS[] = UART_BASE_PTRS;
	UART_Type * p2uart;
	int i = 0;
	uint8_t cantTX = 0;
	_Bool uartWasSleeping = false;
	if ((id >= 0) && (id < UART_CANT_IDS)) //solo se hace algo si el id es válido
			{
		p2uart = UART_POINTERS[id];
		if (uartMode[id] == BLOCKING) {
			while (i < cant)	 //me queda algo por transmitir?
			{
				if ((p2uart->S1 & UART_S1_TDRE_MASK)) //puedo transmitir?
				{
					p2uart->D = msg[i];
					i++;
				}
			}
			cantTX = cant;
		} else if ((uartMode[id] == NON_BLOCKING)) {
			if (lens_TX_buffer[id] == 0) //no habia nada para transmitiendose, recuerdo que debo iniciar la transmision
					{
				uartWasSleeping = true;
			}
			cantTX = TX_BUFFER_LEN - lens_TX_buffer[id]; //cantTX hasta aqui es la cantidad disponible
			cantTX = ((cant <= cantTX) ? cant : cantTX); //cantTX es el minimo entre la cantidad disponible y la cantidad requerida
			if (cantTX == cant) {
				copyTXmsg(id, msg, cantTX); //se copian cantTX chars del mensaje
				if ((uartWasSleeping || ~(p2uart->C2 & UART_C2_TCIE_MASK))
						&& (cantTX > 0)) //si la uart estaba durmiendo desde un principio o estaba activaba pero ahora ya termino
						{
					p2uart->C2 |= UART_C2_TE_MASK;
					p2uart->C2 |= UART_C2_TIE_MASK; //habilito la interrupcion para el fin de transmisión

				}
			} else {
				cantTX = 0;
			}
		}
	}
	return cantTX;
}

uint8_t uartIsTxMsgComplete(uint8_t id) {

	UART_Type * p2uart;
	uint8_t ret = 0;
	if ((id >= 0) && (id < UART_CANT_IDS)) //solo se hace algo si el id es válido
			{
		p2uart = UART_POINTERS[id];
		ret = p2uart->C2 & UART_C2_TCIE_MASK;
	}
	return ret;
}

void flushRXbuffer(uint8_t id) {
	if (uartMode[id] != BLOCKING) {
		markers_RX_buffer[id] = -1;
	}
}

void copyTXmsg(uint8_t id, const char * msg, uint8_t cant) {
	uint8_t i = 0;
	for (i = 0; i < cant; i++) {
		TX_buffers[id][in_markers_TX_buffer[id]] = msg[i];
		if (in_markers_TX_buffer[id] < TX_BUFFER_LEN - 1) {
			in_markers_TX_buffer[id]++; //aumento el in siempre que no llegue al final del arreglo
		} else {
			in_markers_TX_buffer[id] = 0; //si llego al final del arreglo vuelvo al principio
		}
		(lens_TX_buffer[id])++;
	}
}

void UARTX_LON_IRQHandler(uint8_t id) {

}
void UARTX_RX_TX_IRQHandler(uint8_t id) {

	UART_Type * p2uart;
	p2uart = UART_POINTERS[id];

	uint8_t i = 0,
	limit_FIFO_K64 = 0, limit_FIFO_soft = 0;
	if (p2uart->S1 & UART_S1_RDRF_MASK) //recibi data
	{
		if (!flags_reading[id]) {
			if (p2uart->PFIFO & UART_PFIFO_RXFE_MASK) //si implemento FIFO interna del microcontrolador, defino los limites de cuantos datos voy a leer
			{
				limit_FIFO_K64 = p2uart->RCFIFO;
				limit_FIFO_soft = RX_BUFFER_LEN - 1 - markers_RX_buffer[id];
			}
			do {
				if (markers_RX_buffer[id] < RX_BUFFER_LEN - 1) {
					(markers_RX_buffer[id])++; //preparo el marker para escribir. El marker queda apuntando a la posición del último dato ingresado.
					RX_buffers[id][(markers_RX_buffer[id])] = p2uart->D;
					counters_RX_failed[id] = 0;
					;
				} else {
					(counters_RX_failed[id])++;
				}
			} while ((i < limit_FIFO_K64) && (i < limit_FIFO_soft)); //se hace una sola vez si no esta implementada FIFO del micro. Si no, se repite segun cuanto haya en la FIFO

		} else {
			//estaban leyendo el buffer que quiero escribir, asi que tendria que avisar que me manden de nuevo lo que me querian mandar.
		}
	} else if (p2uart->S1 & UART_S1_TDRE_MASK) //se termino una transmisión
	{
		if (lens_TX_buffer[id] > 0) //si queda algo por transmitir
				{

			do {
				p2uart->D = TX_buffers[id][out_Markers_TX_buffer[id]]; //transmito otro char del buffer
				if (out_Markers_TX_buffer[id] < TX_BUFFER_LEN - 1) //actualizo el outMarker
						{
					(out_Markers_TX_buffer[id])++;
				} else {
					out_Markers_TX_buffer[id] = 0;
				}
				lens_TX_buffer[id]--; //actualizo el len ya que termino un mensaje
				i++;
			} while ((i < limit_FIFO_K64) && (i < limit_FIFO_soft)); //se repite el bloque solo si esta implementada la fifo de k64

		} else //si no queda nada por transmitir deshabilito transmisión
		{
			p2uart->C2 = (p2uart->C2 & (~UART_C2_TE_MASK));
			p2uart->C2 = p2uart->C2 & (~UART_C2_TIE_MASK);

		}

	}

}

void UARTX_ERR_IRQHandler(uint8_t id) {

}

void UART0_LON_IRQHandler(void) {
	UARTX_LON_IRQHandler(U0);
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


void UART_setBaudRate(UART_Type * p2uart, uint32_t baudRate) {
	uint8_t brfa;
	uint16_t sbr;
	uint32_t clock;

	clock = ((p2uart == UART0) || (p2uart == UART1)) ?
			(__CORE_CLOCK__) : (__CORE_CLOCK__ >> 1); //CORE_CLOCK o BUS_CLOCK

	if ((baudRate <= MIN_BAUD_RATE) || (baudRate >= MAX_BAUD_RATE)) {
		baudRate = BAUD_RATE_DEFAULT;    //si el baudRate ingresado no es valido
	}									//lo cambio por uno valido por default

	sbr = clock / (baudRate << 4); //clock /(baudRate * 16)
	brfa = ((clock << 1) / baudRate) - (sbr << 5); //(2*clock/baudRate) - 32*sbr

	p2uart->BDH = UART_BDH_SBR((sbr >> 8)); //primero se escribe parte alta
	p2uart->BDL = UART_BDL_SBR(sbr);  //luego se escribe parte baja
	p2uart->C4 = (p2uart->C4 & (~UART_C4_BRFA_MASK)) | UART_C4_BRFA(brfa);

}






