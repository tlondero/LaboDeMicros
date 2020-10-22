/***************************************************************************/ /**
  @file     timer.c
  @brief    Timer driver. Advance implementation
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "../headers/timer.h"
#include "../headers/SysTick.h"
#include <stdbool.h>
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define TIMER_DEVELOPMENT_MODE 1

#define TIMER_ID_DELAY 0 //ID del timer bloqueante reservado dentro del driver

#define TIMER_TICK_MS 1

#if TIMER_TICK_MS != 10000U / (SYSTICK_ISR_FREQUENCY_HZ)
#error Las frecuencias no coinciden!!
#endif // TIMER_TICK_MS != (10000U/SYSTICK_ISR_FREQUENCY_HZ)

#define TIMERS_MAX_CANT 35

#define TIMER_RUNNING 1
#define TIMER_STOPED 0

#define TIMER_EXPIRED 1
#define TIMER_NOT_EXPIRED 0

#define END_OF_TIMER 0

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef struct
{
    ttick_t period;
    ttick_t cnt;
    tim_callback_t callback;
    uint8_t mode : 1;    //TIM_MODE_SINGLESHOT and TIM_MODE_PERIODIC
    uint8_t running : 1; //TIMER_RUNNING and TIMER_STOPED
    uint8_t expired : 1; //TIMER_EXPIRED and TIMER_ALIVE
    uint8_t unused : 5;

} timer_t;

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH LOCAL SCOPE
 ******************************************************************************/
static uint8_t index;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/**
 * @brief Periodic service
 */
static void timer_isr(void);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static timer_t timers[TIMERS_MAX_CANT];
static tim_id_t timers_cant = TIMER_ID_DELAY + 1; //1

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void timerInit(void)
{
    static bool yaInit = false;
    if (yaInit)
        return;

    SysTick_Init(timer_isr); // init peripheral

    yaInit = true;
}

tim_id_t timerGetId(void)
{
#ifdef TIMER_DEVELOPMENT_MODE
    if (timers_cant >= TIMERS_MAX_CANT)
    {
        return TIMER_INVALID_ID;
    }
    else
#endif // TIMER_DEVELOPMENT_MODE
    {
        return timers_cant++;
    }
}

void timerStart(tim_id_t id, ttick_t ticks, uint8_t mode, tim_callback_t callback)
{
#if TIMER_DEVELOPMENT_MODE
    if ((id < timers_cant) && (mode < CANT_TIM_MODES) && (id >= 0))
#endif // TIMER_DEVELOPMENT_MODE
    {
        timers[id].period = ticks; //
        timers[id].cnt = timers[id].period;
        timers[id].mode = mode;
        timers[id].callback = callback;
        timers[id].running = TIMER_RUNNING;
        timers[id].expired = TIMER_NOT_EXPIRED;
    }
}

void timerStop(tim_id_t id)
{
// ****** COMPLETAR ******
#if TIMER_DEVELOPMENT_MODE
    if ((id < timers_cant) && (id > 0))
#endif
    {
        timers[id].running = TIMER_STOPED; //stop timer.
    }
}

//Set timer on TIMER_RUNNING if it was previously stopped
void timerResume(tim_id_t id)
{
#if TIMER_DEVELOPMENT_MODE
    if ((id < timers_cant) && (id > 0))
#endif
        timers[id].running = TIMER_RUNNING;
}

//Reset ticks count on a timer
void timerReset(tim_id_t id)
{
#if TIMER_DEVELOPMENT_MODE
    if ((id < timers_cant) && (id > 0))
#endif
    {
        timers[id].cnt = timers[id].period;
        timers[id].expired = TIMER_NOT_EXPIRED;
        timers[id].running = TIMER_RUNNING;
    }
}

bool timerExpired(tim_id_t id)
{
#if TIMER_DEVELOPMENT_MODE
    if ((id < timers_cant) && (id >= 0))
#endif
    {
        if (timers[id].cnt == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    // Verifico si expiró el timer
    // y bajo el flag
}

//Blocking delay
void timerDelay(ttick_t ticks)
{
    timerStart(TIMER_ID_DELAY, ticks, TIM_MODE_SINGLESHOT, NULL);
    while (!timerExpired(TIMER_ID_DELAY))
    {
        //timers[TIMER_ID_DELAY].cnt-=1;
    }
}

uint8_t isTimerPaused(tim_id_t id)
{
#if TIMER_DEVELOPMENT_MODE
    if ((id < timers_cant) && (id >= 0))
#endif
    {
        if (timers[id].running == TIMER_RUNNING)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void timer_isr(void)
{
    for (index = TIMER_ID_DELAY; index < timers_cant; index++)
    {
        if ((timers[index].running == TIMER_RUNNING) && (timers[index].expired == TIMER_NOT_EXPIRED) && (timers[index].cnt > 0))
        {
            timers[index].cnt -= 1;
            if (timers[index].cnt == END_OF_TIMER)
            { //end of timer --> perform callback
                if (timers[index].callback != NULL)
                {
                    timers[index].callback();
                }

                timers[index].expired = TIMER_EXPIRED;
                if (timers[index].mode == TIM_MODE_SINGLESHOT)
                {
                    timers[index].running = TIMER_STOPED;
                }
            }
        }
        //If periodic timer is expired perform reset.
        else if ((timers[index].running == TIMER_RUNNING) && (timers[index].expired == TIMER_EXPIRED) && (timers[index].mode == TIM_MODE_PERIODIC))
        {
            timers[index].cnt = timers[index].period;
            timers[index].expired = TIMER_NOT_EXPIRED;
        }
    }
}

/******************************************************************************/
