#include "encoder.h"
#include <stdbool.h>
#include "timer.h"
#include <math.h>
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define MAX_ENCODERS 1
#define NO_MORE_ENCODERS 255
#define DEVELOPMENT_MODE 1
/*******************************************************************************
 * STATIC VARIABLES
 ******************************************************************************/
static encoder_state_t encoders[MAX_ENCODERS];
static uint16_t enconders_cant = 0;

/*******************************************************************************
 * FUNCTION DEFINITIONS
 ******************************************************************************/

void encoder_init(void)
{
    static bool warm_up_rdy = false;
    if (!warm_up_rdy)
    {
        warm_up_rdy = true;
        timerInit();
    }
}

encoder_id encoder_register(pin_t pin_A, pin_t pin_B)
{
    encoder_id id = enconders_cant++;
#if DEVELOPMENT_MODE
    if (enconders_cant < MAX_ENCODERS)
#endif
    {

        encoders[id].current_state_A = LOW;
        encoders[id].current_state_B = LOW;
        encoders[id].prev_state_A = LOW;
        encoders[id].prev_state_A = LOW;
        encoders[id].finite_count = FINITE; // default
        encoders[id].max = 100;
        encoders[id].min = 0;
        encoders[id].clockwise = CLOCKWISE;

        gpioMode(pin_A, INPUT);
        gpioIRQ(pin_A, GPIO_IRQ_MODE_BOTH_EDGES, encoder_update);
        gpioMode(pin_B, INPUT);
    }
#if DEVELOPMENT_MODE
    else
#endif
    {
        id = NO_MORE_ENCODERS;
    }
    return id;
}

void encoder_set_maxmin(encoder_id id, int16_t min, int16_t max)
{
#if DEVELOPMENT_MODE
    if (id < enconders_cant)
#endif
    {
        encoders[id].max = max;
        encoders[id].min = min;
    }
}

void encoder_reset_cnt(encoder_id id)
{
#if DEVELOPMENT_MODE
    if (id < enconders_cant)
#endif
    {
        encoders[id].count = 0;
    }
}

uint32_t encoder_get_count(encoder_id id)
{
#if DEVELOPMENT_MODE
    if (id < enconders_cant)
#endif
    {
        return encoders[id].count;
    }
}

//Callback for timer
void enconder_update(void)
{
    int i = 0;
    for (i = 0; i < enconders_cant; i++)
    {
        //READ GPIOS INPUT PINS and UPDATE CURRENT STATE
        // encoders[i].current_state_A = gpioRead(encoders[i].pin_A);
        encoders[i].current_state_B = gpioRead(encoders[i].pin_B);
        //COMPARE PREV AND CURRENT STATE
        if (encoders[i].current_state_B == HIGH)
        {
            encoders[i].count += 1;
        }
        else
        {
            encoders[i].count -= 1;
        }
    }
}