/*
	To assemble a  package the first thing to have in mind is that the package lenght is fixed at 4 binits.
	you will have to zero padd the binits not used.
	Header- num_data (if there is any) - zero padd.

Package header:
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
		binits  :  3

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
#define PIECE_L_H ('J')
#define PIECE_L_H ('T')
#define PIECE_L_H ('I')
#define PIECE_L_H ('Z')
#define PIECE_L_H ('O')
#define PIECE_L_H ('S')
