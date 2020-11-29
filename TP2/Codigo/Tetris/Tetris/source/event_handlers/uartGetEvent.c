/***************************************************************************//**
 @file     uartGetEvent.c
 @brief    Application functions
 @author   MAGT
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <header/drivers/uart.h>
#include <header/event_handlers/uartGetEvent.h>
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/



/*******************************************************************************
 * FUNCTION PROTOTYPES WITH FILE SCOPE DECLARATION
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE DEFINITION
 *******************************************************************************/
void uartGetEvent(package * data, uint8_t id){
	static char msg[1000];

	if (uartIsRxMsg(id) && (uartGetRxMsgLength(id) > PACKAGE_SIZE)) {
		uint16_t len = 0;
		len = uartReadMsg(id, msg, PACKAGE_SIZE);
		switch (msg[0]) {
		case BRIGHTNESS_H:
#if DEV_MODE
			uartWriteMsg(id, "Brightness changed\r\n", 21);
#endif
			data->birghtness.birghtness=msg[1];
			data->birghtness.change=true;
			break;
		case FALL_H:
#if DEV_MODE
			uartWriteMsg(id, "Fall speed changed\r\n", 21);
#endif
			data->fall_speed.fall_speedowagon=msg[1];
			data->fall_speed.change=true;
			break;
		case MOV_LEFT_H:
#if DEV_MODE
			uartWriteMsg(id, "Action Left\r\n", 14);
#endif
			data->action.left=true;
			break;
		case MOV_RIGHT_H:
#if DEV_MODE
			uartWriteMsg(id, "Action Right\r\n", 15);
#endif
			data->action.right=true;
			break;
		case MOV_DOWN_H:
#if DEV_MODE
			uartWriteMsg(id, "Action Down\r\n", 14);
#endif
			data->action.down=true;
			break;
		case MOV_ROTATE_H:
#if DEV_MODE
			uartWriteMsg(id, "Action Rotate\r\n", 16);
#endif
			data->action.rotate=true;
			break;
		case RESET_H:
#if DEV_MODE
			uartWriteMsg(id, "Action Reset\r\n", 15);
#endif
			data->reset=true;
			break;
		case PIECE_L_H:
			data->piece.changed=true;
			data->piece.piece='L';
			data->piece.color.r=msg[1];
			data->piece.color.g=msg[2];
			data->piece.color.b=msg[3];
#if DEV_MODE
			uartWriteMsg(id, "Piece L prop changed\r\n", 23);
#endif
			break;
		case PIECE_J_H:
			data->piece.changed=true;
			data->piece.piece='H';
			data->piece.color.r=msg[1];
			data->piece.color.g=msg[2];
			data->piece.color.b=msg[3];
#if DEV_MODE
			uartWriteMsg(id, "Piece J prop changed\r\n", 23);
#endif
			break;
		case PIECE_T_H:
			data->piece.changed=true;
			data->piece.piece='T';
			data->piece.color.r=msg[1];
			data->piece.color.g=msg[2];
			data->piece.color.b=msg[3];
#if DEV_MODE
			uartWriteMsg(id, "Piece T prop changed\r\n", 23);
#endif
			break;
		case PIECE_I_H:
			data->piece.changed=true;
			data->piece.piece='I';
			data->piece.color.r=msg[1];
			data->piece.color.g=msg[2];
			data->piece.color.b=msg[3];
#if DEV_MODE
			uartWriteMsg(id, "Piece I prop changed\r\n", 23);
#endif
			break;
		case PIECE_Z_H:
			data->piece.changed=true;
			data->piece.piece='Z';
			data->piece.color.r=msg[1];
			data->piece.color.g=msg[2];
			data->piece.color.b=msg[3];
#if DEV_MODE
			uartWriteMsg(id, "Piece Z prop changed\r\n", 23);
#endif
			break;
		case PIECE_O_H:
			data->piece.changed=true;
			data->piece.piece='O';
			data->piece.color.r=msg[1];
			data->piece.color.g=msg[2];
			data->piece.color.b=msg[3];
#if DEV_MODE
			uartWriteMsg(id, "Piece O prop changed\r\n", 23);
#endif
			break;
		case PIECE_S_H:
			data->piece.changed=true;
			data->piece.piece='S';
			data->piece.color.r=msg[1];
			data->piece.color.g=msg[2];
			data->piece.color.b=msg[3];
#if DEV_MODE
			uartWriteMsg(id, "Piece S prop changed\r\n", 23);
#endif
			break;
		default:
			break;
		}
	}

}
/*******************************************************************************
 * FUNCTION PROTOTYPES WITH FILE SCOPE DEFINITION
 *******************************************************************************/








