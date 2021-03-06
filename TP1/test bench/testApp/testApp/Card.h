#pragma once
#include <cstdint>
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define PAN_LENGHT (19)
 /*******************************************************************************
  * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
  ******************************************************************************/
  /*******************************************************************************
   * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
   ******************************************************************************/

   /*******************************************************************************
    * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
    ******************************************************************************/
    /**
     * @brief  cardGetPAN: returns a pointer to the data, its null if it doesnt have a message, or the message had an error.
     * @returns pointer to the data.
     */
uint8_t* cardGetPAN(void);

/**
 * @brief  cardInitDriver: Initializes all the drivers that the card driver uses.
 */
void cardInitDriver(void (*fun_callback)(void));