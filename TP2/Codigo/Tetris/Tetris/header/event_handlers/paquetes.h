/*
	To assemble a  package the first thing to have in mind is that the package lenght is fixed at 4 binits.
	you will have to zero padd the binits not used.
	Header- num_data (if there is any) - zero padd.

Packages:
	Brightness package:
		header :   'B'//Brightness
		num data:  ascii character that ranges from 0 to 100.
		binits  :  2


	Fall package:
		header  :  'F' //Fall
		num data:  ascii charater that ranges from 0 to 100
		binits  :  2

	Movement package:
		header  : 'E' or 'D' or 'A' or 'G' //Eizquierda, Derecha, Abajo, Girar
		binits  :  1

	Reset package:
		header  :  'R'//Reset
		binits  :  3

	Color package:
		header  :  'L' or 'J' or 'T' or 'I' or 'Z' or 'O' or 'S'
		num data:  3 ascii character that ranges from 0 to 255 RGB
		binits  :  4

*/
#ifndef paquetes_H
#define paquetes_H
#include <stdbool.h>
#include <stdint.h>

#define BRIGHTNESS_H ('B')

#define FALL_H ('F')

#define MOV_LEFT_H ('E')
#define MOV_RIGHT_H ('D')
#define MOV_DOWN_H ('A')
#define MOV_ROTATE_H ('G')

#define RESET_H ('R')

#define PIECE_L_H ('L')
#define PIECE_J_H ('J')
#define PIECE_T_H ('T')
#define PIECE_I_H ('I')
#define PIECE_Z_H ('Z')
#define PIECE_O_H ('O')
#define PIECE_S_H ('S')
#define PKG_CANT_PIECES 7

typedef struct{
	uint8_t r;
	uint8_t g;
	uint8_t b;
}color_t;
typedef struct{
	char piece;
	color_t color;
	bool changed;
}piece_prop;

typedef struct{
	bool left;
	bool right;
	bool down;
	bool rotate;
}piece_movement;

typedef struct{
	uint8_t birghtness;
	bool change;
}brightness_t;
typedef struct{
	uint8_t fall_speedowagon;
	bool change;
}fall_t;

typedef struct{
	brightness_t birghtness;
	fall_t fall_speed;
	piece_movement action;
	piece_prop piece;
	bool reset;
	bool pause;
}package;
#endif /* paquetes_H */
