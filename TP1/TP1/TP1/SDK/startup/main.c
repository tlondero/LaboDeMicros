/***************************************************************************//**
  @file     main.c
  @brief    FW main
  @author   Nicolás Magliola
 ******************************************************************************/

#include "hardware.h"

void App_Init (void);
void App_Run (void);

void FRDM_tb_Init(void);
void FRDM_tb_Run(void);


int main (void)
{
    hw_Init();
    hw_DisableInterrupts();
    //App_Init(); /* Program-specific setup */
    FRDM_tb_Init();
    hw_EnableInterrupts();

    __FOREVER__
        //App_Run(); /* Program-specific loop  */
		FRDM_tb_Run();
}
