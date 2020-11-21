/***************************************************************************/ /**
  @file     uart.h
  @brief
  @author   MAGT
 ******************************************************************************/

#ifndef UART_H_
#define UART_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define UART_CANT_IDS 5



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
enum {U0, U1, U2, U3, U4};
enum {BLOCKING, NON_BLOCKING};
enum{NO_PARITY_UART,EVEN_PARITY_UART ,ODD_PARITY_UART};
enum{NBITS_8=8,NBITS_9};

typedef struct {
    uint32_t baudRate;
    uint8_t mode;
    uint8_t parity;
    uint8_t nBits;
} uart_cfg_t;



/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initialize UART driver
 * @param id UART's number
 * @param config UART's configuration (baudrate, parity, etc.)
*/
void uartInit (uint8_t id, uart_cfg_t config);

/**
 * @brief Check if a new byte was received
 * @param id UART's number
 * @return A new byte has being received
*/
bool uartIsRxMsg(uint8_t id);

/**
 * @brief Check how many bytes were received
 * @param id UART's number
 * @return Quantity of received bytes
*/
uint32_t uartGetRxMsgLength(uint8_t id);

/**
 * @brief Read a received message. Non-Blocking
 * @param id UART's number
 * @param msg Buffer to paste the received bytes
 * @param cant Desired quantity of bytes to be pasted
 * @return Real quantity of pasted bytes
*/
uint32_t uartReadMsg(uint8_t id, char* msg, uint32_t cant);

/**
 * @brief Write a message to be transmitted. Non-Blocking
 * @param id UART's number
 * @param msg Buffer with the bytes to be transfered
 * @param cant Desired quantity of bytes to be transfered
 * @return Real quantity of bytes to be transfered
*/
uint32_t uartWriteMsg(uint8_t id, const char* msg, uint32_t cant);
/**
 * @brief Check if all bytes were transfered
 * @param id UART's number
 * @return All bytes were transfered
*/
uint32_t uartIsTxMsgComplete(uint8_t id);



/*******************************************************************************
 ******************************************************************************/

#endif /* UART_H_ */
