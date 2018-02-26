/*
 * cc1190.c
 *
 *  Created on: Feb 21, 2018
 *      Author: KB1LQ
 */

#include "driverlib.h"
#include "cc1190.h"


void RfPowerAmplifierEnable(void){
    GPIO_setOutputHighOnPin(
        GPIO_PORT_P4,
        PA_ENABLE
        );
}


void RfPowerAmplifierDisable(void){
    GPIO_setOutputLowOnPin(
        GPIO_PORT_P4,
        PA_ENABLE
        );
}

void RfLowNoiseAmplifierEnable(void){
    GPIO_setOutputHighOnPin(
        GPIO_PORT_P4,
        LNA_ENABLE
        );
}


void RfLowNoiseAmplifierDisable(void){
    GPIO_setOutputLowOnPin(
        GPIO_PORT_P4,
        LNA_ENABLE
        );
}

void RfHighGainModeEnable(void){
    GPIO_setOutputHighOnPin(
        GPIO_PORT_P4,
        HGM_SELECT
        );
}


void RfHighGainModeDisable(void){
    GPIO_setOutputLowOnPin(
        GPIO_PORT_P4,
        HGM_SELECT
        );
}
