#include "uart.h"
#define DEVELOPMENT_MODE 1

#define UART0_CLOCK 100000000.0
#define COMPUTE_SBR(bd) UART0_CLOCK / (16.0 * bd)
#define LOW_16BIT_MASK 0x00FF
#define HIGH_16BIT_MASK 0xFF00

#define SBR2BDH(num) (uint8_t)(((uint16_t)num & HIGH_16BIT_MASK) >> 8)
#define SBR2BDL(num) (uint8_t)(((uint16_t)num & LOW_16BIT_MASK))
#define SBR2BRFD(num) (uint8_t)(((num - (uint16_t)num)) * 1000)

void uartInit(uint8_t id, uart_cfg_t config)
{

	if (id <= 5 && id >= 0)
	{
		UART_Type *UART0 = UART_BASE_PTRS;
		sim->SCGC4 |= SIM_SCGC4_UART0_MASK;					 // clock gating
		UART0->C2 &= (~(UART_C2_TE_MASK | UART_C2_RE_MASK)); //para configurar correctamente deshabilito la transmision y la recepcion
		UART_setBaudRate(UART0, config.baudRate);
		UART_setParity(UART0, config.parity);
		UART0->BDH &= ~UART_BDH_SBNS_MASK;
	}
}

void UART_setBaudRate(uint16_t baudRate) //Tiene que recibir tambien un puntero al uart
{
	//Compute SBR with assuming BRFD equal to zero
	float sbr = COMPUTE_SBR(baudRate);
	//Split SBR into BDH and BDL
	//BDH
	uint8_t bdh = SBR2BDH(sbr);
	//BDL
	uint8_t bdl = SBR2BDL(sbr);
	//Calculate BRFD
	float brfd = SBR2BRFD(sbr);
}
