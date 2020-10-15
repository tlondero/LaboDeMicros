
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include "encoder_tb/encoder_tb.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


void App_Init (void)
{
	EncoderInit();
	encoder_test_infinite_count_cc();

}

void App_Run (void)
{
	do_always();
}


