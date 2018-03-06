/**
 * @file cc1190.c
 * @brief This file controls the radio hardware of the CC1190 transmitter amplifier
 * and reciever low noise amplifier on Faraday.
 * @author Brenton Salmi, KB1LQD
 *
 * @date 2/25/2018
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
