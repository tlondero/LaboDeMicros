
#ifndef UART_H_
#define UART_H_


void UART_Init (void);
void UART_SetBaudRate (UART_Type *uart, uint32_t baudrate);
unsigned char UART_Get_Status(void);
unsigned char UART_Get_Data(void);
void UART_Send_Data(unsigned char txdata);


#endif /* UART_H_ */
