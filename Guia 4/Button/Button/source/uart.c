/*
 * uart.c
 *
 *  Created on: Oct 30, 2020
 *      Author: Guido
 */
#include "uart.h"
#include "MK64F12.h"
#include "stdint.h"
void uart_init (UART_Type * uartch, int sysclk, int baud){
 register uint16 ubd, brfa;
 uint8 temp;

 /* Enable the clock to the selected UART */
 if(uartch == UART0_BASE_PTR)
 SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
 else
 if (uartch == UART1_BASE_PTR)
 SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;
 else
 if (uartch == UART2_BASE_PTR)
 SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
 else
 if(uartch == UART3_BASE_PTR)
 SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;
 else
 if(uartch == UART4_BASE_PTR)
 SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;
 else
 SIM_SCGC1 |= SIM_SCGC1_UART5_MASK;

 /* Make sure that the transmitter and receiver are disabled while we
 * change settings.
 */
 UART_C2_REG(uartch) &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
 /* Configure the UART for 8-bit mode, no parity */
 /* We need all default settings, so entire register is cleared */
 UART_C1_REG(uartch) = 0;

 /* Calculate baud settings */
 ubd = (uint16)((sysclk*1000)/(baud * 16));

 /* Save off the current value of the UARTx_BDH except for the SBR */
 temp = UART_BDH_REG(uartch) & ~(UART_BDH_SBR(0x1F));

 UART_BDH_REG(uartch) = temp | UART_BDH_SBR(((ubd & 0x1F00) >> 8));
 UART_BDL_REG(uartch) = (uint8)(ubd & UART_BDL_SBR_MASK);

 /* Determine if a fractional divider is needed to get closer to the baud rate */
 brfa = (((sysclk*32000)/(baud * 16)) - (ubd * 32));

 /* Save off the current value of the UARTx_C4 register except for the BRFA */
 temp = UART_C4_REG(uartch) & ~(UART_C4_BRFA(0x1F));

 UART_C4_REG(uartch) = temp | UART_C4_BRFA(brfa);
 /* Enable receiver and transmitter */
 UART_C2_REG(uartch) |= (UART_C2_TE_MASK | UART_C2_RE_MASK );
}

char uart_getchar (UART_Type * channel)
{
 /* Wait until character has been received */
 while (!(UART_S1_REG(channel) & UART_S1_RDRF_MASK));

 /* Return the 8-bit data from the receiver */
 return UART_D_REG(channel);
}
void uart_putchar (UART_Type * channel, char ch)
{
 /* Wait until space is available in the FIFO */
 while(!(UART_S1_REG(channel) & UART_S1_TDRE_MASK));

 /* Send the character */
 UART_D_REG(channel) = (uint8)ch;
 }
