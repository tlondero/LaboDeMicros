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
		num data:  3 ascii character that ranges from 0 to 255
		binits  :  4

*/

#define BRIGHTNESS_H ('B')

#define FALL_H ('F')

#define MOV_LEFT_H ('E')
#define MOV_RIGHT_H ('D')
#define MOV_UP_H ('A')
#define MOV_ROTATE_H ('G')

#define RESET_H ('R')

#define PIECE_L_H ('L')
#define PIECE_J_H ('J')
#define PIECE_T_H ('T')
#define PIECE_I_H ('I')
#define PIECE_Z_H ('Z')
#define PIECE_O_H ('O')
#define PIECE_S_H ('S')
