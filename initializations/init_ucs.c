/*
 * init_ucs.c
 *
 *  Created on: Feb 7, 2018
 *      Author: KB1LQ
 */




//******************************************************************************
#include "driverlib.h"
#include "init_ucs.h"
//*****************************************************************************


//Target frequency for MCLK in kHz
//
//*****************************************************************************
#define UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ   12000

//*****************************************************************************
//
//MCLK/FLLRef Ratio
//
//*****************************************************************************
#define UCS_MCLK_FLLREF_RATIO   366




unsigned char initialize_ucs(void){

    //Set VCore = 1 for 12MHz clock
    PMM_setVCore(PMM_CORE_LEVEL_1);

    //Set DCO FLL reference = REFO
    UCS_initClockSignal(
        UCS_FLLREF,
        UCS_REFOCLK_SELECT,
        UCS_CLOCK_DIVIDER_1
        );
    //Set ACLK = REFO
    UCS_initClockSignal(
        UCS_ACLK,
        UCS_REFOCLK_SELECT,
        UCS_CLOCK_DIVIDER_1
        );

    //Set Ratio and Desired MCLK Frequency  and initialize DCO
    UCS_initFLLSettle(
        UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ,
        UCS_MCLK_FLLREF_RATIO
        );

    // Enable global oscillator fault flag
    SFR_clearInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
    SFR_enableInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);



    //Verify if the Clock settings are as expected
    volatile uint32_t clockValue = 0;
    clockValue = UCS_getSMCLK();
    clockValue = UCS_getMCLK();
    clockValue = UCS_getACLK();

    return 0;
}
