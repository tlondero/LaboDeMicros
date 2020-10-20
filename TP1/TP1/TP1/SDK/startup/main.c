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

void PV_tb_Init(void);
void PV_tb_Run(void);

void FTM_TB_APP_RUN(void);
void FTM_TB_APP_INIT(void);

void LED_TB_APP_INIT(void);
void LED_TB_APP_RUN(void);

int main (void)
{
    hw_Init();
    hw_DisableInterrupts();
    App_Init(); /* Program-specific setup */
    //FRDM_tb_Init();
   // PV_tb_Init();
    //LED_TB_APP_INIT();
    hw_EnableInterrupts();

    while(1){
        App_Run(); /* Program-specific loop  */
		//FRDM_tb_Run();
		//PV_tb_Run();
		//LED_TB_APP_RUN();
    }
}
