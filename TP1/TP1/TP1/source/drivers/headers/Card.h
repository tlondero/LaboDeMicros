/***************************************************************************/ /**
  @file     Card.h
  @brief    Driver de lector de tarjeta magnetica
  @author   MAGT
 ******************************************************************************/

#ifndef CARD_H_
#define CARD_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define PAN_LENGHT (19)

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief  cardGetPAN: returns a pointer to the data, its null if it doesnt have a message, or the message had an error.
 * @returns pointer to the data.
 */
uint8_t *cardGetPAN(void);

/**
 * @brief  cardInitDriver: Initializes all the drivers that the card driver uses.
 */
void cardInitDriver(void (*fun_callback)(void));

/*******************************************************************************
 ******************************************************************************/

#endif /* CARD_H_ */
