
/***************************************************************************//**
  @file     fsm.h
  @brief    fsm that controls the system
  @author   MAGT
 ******************************************************************************/

#ifndef APP_HEADERS_UTILITY_H_
#define APP_HEADERS_UTILITY_H_
 /*******************************************************************************
  * INCLUDE HEADER FILES
  ******************************************************************************/
#include <stdint.h>
 /*******************************************************************************
  * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
  ******************************************************************************/
  /**
   * @brief transformToNum: transforms the uint8_t array to a number of count digits.
   *@param data: pointer to the array to convert
   *@param count, number of digits of the array number
   * @return the number formed by the array of count digits.
   */

uint32_t transformToNum(uint8_t* data, uint8_t count);
/**
 * @brief num2str: transforms uint32_t number to a the uint8_t array of longitude 8.
 *@param num: number to convert
 * @return  pointer to the array to converted
 */
char* num2str(uint32_t num);
/**
 * @brief pwr: power operation.
 *@param num & exp.
 * @return  "num to the exp"
 */

uint32_t pwr(uint32_t num, uint32_t exp);
#endif /* APP_HEADERS_UTILITY_H_ */
