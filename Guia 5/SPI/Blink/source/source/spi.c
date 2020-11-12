#include "spi.h"
#include <stdint.h>
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define SPI_MODE 2

// 200 Kbauds == SysClock/BR = 1Meg/5 = 200Khz
#define SPI_BR_CNFG 5
#define SPI_PBR_CNFG 1
/*******************************************************************************
 * VARIABLES WITH LOCAL SCOPE USING STATIC 
 ******************************************************************************/
static spi_id_t spis = 0
/*******************************************************************************
 * ENUMERATIONS, STRUCTURES AND TYPEDEFS
 ******************************************************************************/
static uint32_t simMasks[] = {SIM_SCGC5_PORTA_MASK, SIM_SCGC5_PORTB_MASK, SIM_SCGC5_PORTC_MASK, SIM_SCGC5_PORTD_MASK, SIM_SCGC5_PORTE_MASK};
static PORT_Type *portPtrs[] = PORT_BASE_PTRS;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH LOCAL SCOPE
 ******************************************************************************/
void init_spi_pins(pin_t pin)
{
    //TODO: chequear por qué hacemos esto
    SIM->SCGC5 |= simMasks[PIN2PORT(pin)];
    //Set pin to the spi alternative. Only SPI0
    PORT_Type *port = portPtrs[PIN2PORT(pin)];
    port->PCR[PIN2NUM(pin)] = 0x00;
    port->PCR[PIN2NUM(pin)] |= PORT_PCR_MUX(SPI_MODE);
    port->PCR[PIN2NUM(pin)] |= PORT_PCR_IRQC(0);
}

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

spi_id_t spi_init(pin_t sck, pin_t miso, pin_t mosi, pin_t pcs)
{
    //Enable clock gating
    SIM->SCGC6 |= SIM_SCGC6_SPI0(1); //Clock gating for SPI0 and SPI1
    // SIM->SCGC3 |= SIM_SCGC3_SPI0(1); //Clock gating for SPI2
    init_spi_pins(sck);
    init_spi_pins(miso);
    init_spi_pins(mosi);
    init_spi_pins(pcs);

    //MCR CONFING
    SPI0->MCR = SPI_MCR_HALT_MASK | SPI_MCR_MSTR_MASK; //Halt communication
    SPI0->MCR = SPI_MCR_MSTR(1) | SPI_MCR_DCONF(0xb00) | SPI_MCR_MTFE(0) | SPI_MCR_PCSSE(0) | SPI_MCR_PCSIS(0) | SPI_MCR_MDIS(0) | SPI_MCR_DIS_TXF(0) | SPI_MCR_DIS_RXF(0);

    //CTAR CONFIG
    SPI0->CTAR =   SPI_CTAR_CPOL(0)
                 | SPI_CTAR_CPHA(0) 
                 | SPI_CTAR_FMSZ(8 - 1)
                 | SPI_CTAR_DBR(0) // 50/50 DUTY CYCLE
                 | SPI_CTAR_PBR(SPI_PBR_CNFG) 
                 | SPI_CTAR_BR(SPI_BR_CNFG)
                 | SPI_CTAR_ASC(SPI_BR_CNFG-3)
                 | SPI_CTAR_PASC(SPI_PBR_CNFG-3)
                 | SPI_CTAR_PCSSCK(SPI_PBR_CNFG)
                 | SPI_CTAR_PDT(0)
                 | SPI_CTAR_DT(0);       

    //Enable Master chip select
    SPI0->MCR |= SPI_MCR_PCSIS(1); //Select slave 

                //Clock enable   //Stop communication  //Enable Master Mode
    SPI0->MCR |= SPI_MCR_MDIS(0) |SPI_MCR_HALT(0) | SPI_MCR_MSTR(1);

    return spis++;
}
