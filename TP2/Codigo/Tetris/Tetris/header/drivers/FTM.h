#ifndef FTM_H_
#define FTM_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define FTM_NO_SPACE -1		//ID de retorno cuando la inicialización falla
#define FTM_CLK 50000000U	//Clock del FTM

#define NXP_K64F_LQFP100 1  //Define para la tabla de pines
#define CALLBACK_MODE 0    	//Si está en 1 se ejecutaran en los IRQHandlers los callbacks provistos.
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

enum channels {FTM_CH_0, FTM_CH_1, FTM_CH_2, FTM_CH_3, FTM_CH_4, FTM_CH_5, FTM_CH_6, FTM_CH_7, CANT_CHANNELS};

/*
 * CONFIGURACIONES DEL MSB:A (MODO DE FUNCIONAMIENTO)
 */
typedef enum
{
	FTM_mInputCapture,
	FTM_mOutputCompare,
	FTM_mPulseWidthModulation,
} FTMMode_t;

/*
 * CONFIGURACIONES DEL ELSB:A PARA MODO INPUT CAPTURE
 */
typedef enum
{
	FTM_eRising 		= 0x01,		//Captura del contador en flanco ascendiente
	FTM_eFalling 		= 0x02,		//Captura del contador en flanco descendiente
	FTM_eEither 		= 0x03,		//Captura del contador en ambos flancos
} FTMEdge_t;

/*
 * CONFIGURACIONES DEL ELSB:A PARA MODO OUTPUT COMPARE
 */
typedef enum
{
	FTM_eToggle 		= 0x01,		//Toggleo del pin utilizado cuando CnV == FTM_CNT
	FTM_eClear 			= 0x02,		//Clear del pin utilizado cuando CnV == FTM_CNT
	FTM_eSet 			= 0x03,		//Set del pin utilizado cuando CnV == FTM_CNT
} FTMEffect_t;

/*
 * CONFIGURACIONES DEL ELSB:A PARA MODO EDGE-ALIGNED PWM
 */
typedef enum
{
	FTM_lAssertedHigh	= 0x02,		//Se fuerza el pin utilizado a alto cuando CnV == FTM_CNT
	FTM_lAssertedLow 	= 0x03,		//Se fuerza el pin utilizado en bajo cuando CnV == FTM_CNT
} FTMLogic_t;


/*
 * CONFIGURACIONES DEL DIVISOR DE FRECUENCIA
 */
typedef enum
{
	FTM_PSC_x1		= 0x00,
	FTM_PSC_x2		= 0x01,
	FTM_PSC_x4		= 0x02,
	FTM_PSC_x8		= 0x03,
	FTM_PSC_x16		= 0x04,
	FTM_PSC_x32		= 0x05,
	FTM_PSC_x64		= 0x06,
	FTM_PSC_x128	= 0x07,

} FTM_Prescal_t;

typedef struct{
	FTMMode_t MODE;				//Modo de funcionamiento

	FTM_Prescal_t PSC;			//Divisor de frecuencia
	FTMEdge_t IC_EDGE;			//En que flanco triggerear con modo IC
	FTMEffect_t OC_EFFECT;		//En que estado forzar el pin cuando se matchea en OC
	FTMLogic_t EPWM_LOGIC;		//En que estado se fuerza el pin cuando se matchea en EPWM

	uint16_t MODULO;			//Cuando FTM_CNT == MODULO se reinicia el FTM_CNT en el valor CNTIN
	uint16_t CNT;			//No utilizado //TODO: Sacar del codigo
	uint16_t CNTIN;				//Valor al que se reiniciara el FTM_CNT cuando FTM_CNT == MODULO
	uint16_t CNV;				//Cuando CNV == FTM_CNT, y si las interrupciones del modulo estan habilitadas,
								//se interrumpirá.
	uint8_t DMA;				//Configuracion de dma

	uint8_t useCallback;		//Enable para utilizar o no el callback provisto
	void (*CALLBACK)(void);		//Callback que se ejecutará en las interrupciones SI EL DEFINE CALLBACK_MODE ESTÁ
								//EN 1 y el enable useCallback está en 1.

}FTM_DATA;
/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/* @brief: Inicializa un pin con las especificaciones pedidas en data.
 * @param  port: el puerto del pin que se quiere inicializar
 * @param  num: el numero del puerto  del pin que se quiere inicializar
 * @param  data: las especificaciones para la inicializacion, ver la estructura FTM_DATA.
 * @returns: id de la FTM. Si hubo error se retorna el define FTM_NO_SPACE.
 */
uint8_t FTMInit(uint8_t port, uint8_t num, FTM_DATA data);

/* @brief: Para el clock de FTM para un modulo entero.
 * @param  module: que modulo se le quiere parar el clock.
 */
void FTMStopClock(uint8_t module);

/* @brief: Continua el clock de FTM para un modulo entero.
 * @param  module: a que modulo se le quiere parar el clock.
 */
void FTMStartClock(uint8_t module);

/* @brief: Set del CnV
 * @param  id: id de la FTM
 * @param cnv
 */
void FTMSetCnV(uint8_t id, uint16_t cnv);

/* @brief: Set del prescaler o divisor de frecuencias
 * @param  id: id de la FTM
 * @param PSC
 */
void FTMSetPSC(uint8_t id, uint16_t PSC);

/* @brief: Set del MODULO
 * @param  id: id de la FTM
 * @param MOD
 */
void FTMSetMOD(uint8_t id, uint16_t MOD);

/* @brief: Habilita (1) o deshabilita (0) las interrupciones
 * @param  id: id de la FTM
 * @param mode: 1 para habilitar, 0 para deshabilitar
 */
void FTMSetInterruptMode(uint8_t id, uint8_t mode);

/* @brief: configura los registros de SYNC y SYNCONF para poder
 * realizar la sincronización por software de la escritura en los registros de MOD, CNV y CNTIN
 * @param  id: id de la FTM
 */
void FTMSetSoftwareSync(uint8_t id);

/* @brief: Set del bit que comienza la sincronización para la escritura
 * en los registros de MOD, CNV y CNTIN
 * @param  id: id de la FTM
 * @param cnv
 */
void FTMSetSWSYNC(uint8_t id);

/* @brief: Set del bit de DMA
 * @param  id: id de la FTM
 * @param mode: 1 o 0
 */
void FTMSetDMA(uint8_t id, uint8_t mode);

#endif /* FTM_H_ */
