/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "drivers/headers/board.h"
#include "drivers/headers/gpio.h"
#include "source/tests/display_tb/display_tb.h"
#include "drivers/headers/encoder.h"
#include "drivers/headers/display.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void delayLoop(uint32_t veces);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
static encoder_id enc_id;

void App_Init (void)
{
    //gpioMode(PIN_LED_BLUE, OUTPUT);
    //test_display029();
    //test_multiplexing(); //passing
	dispInit();
	enc_id = EncoderRegister(PIN_C2,PIN_C7);
	dispSendChar('5', 0);

}

/* Función que se llama constantemente en un ciclo infinito */
static uint8_t pos = 0;
static event_t ev = -1;
void App_Run (void)
{

	if(EncoderEventAVB(enc_id) == EVENT_AVB){
		ev = EncoderPopEvent(enc_id);
		if(ev == RIGHT_TURN && pos < 4)
			pos++;
		else if (ev == LEFT_TURN && pos > 0)
			pos--;
		//dispClearAll();
		dispSelect(pos);

	}


}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void delayLoop(uint32_t veces)
{
    while (veces--);
}


/*******************************************************************************
 ******************************************************************************/
