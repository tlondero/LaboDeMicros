/***************************************************************************/ /**
  @file     uartPackHand.h
  @brief
  @author   MAGT
 ******************************************************************************/

#ifndef UARTPACKHAND_H_
#define UARTPACKHAND_H_
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define PACKAGE_SIZE (4)


#define DEV_MODE 1


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef struct{
	uint8_t r;
	uint8_t g;
	uint8_t b;
}color_t;
typedef struct{
	char piece;
	color_t color;
}piece_prop;

typedef struct{
	bool left;
	bool right;
	bool down;
	bool rotate;
}piece_movement;
typedef struct{
	uint8_t birghtness;
	uint8_t fall_speed;
	piece_movement action;
	piece_prop piece;
	bool reset;
}package;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief interprets the packages specified by the "paquetes.h" file and fills the corresponding data structure acordingly
 * @param
 * @data * is a pointer tpo the package data structure to be filled
 * @id UART's number
 * @return A new byte has being received
*/
void uartPackProcess(package * data, uint8_t id );

#endif /* UARTPACKHAND_H_ */
