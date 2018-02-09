/*
 * init_spi.c
 *
 *  Created on: Feb 9, 2018
 *      Author: KB1LQ
 */

#include "driverlib.h"
#include "init_spi.h"

/**
 * Specify desired frequency of SPI communication
 */
#define SPICLK                          500000

uint8_t transmitData = 0x00;
uint8_t receiveData = 0x00;
uint8_t returnValue = 0x00;


unsigned char init_spi(void){
    //Initialize Master
    USCI_B_SPI_initMasterParam param = {0};
    param.selectClockSource = USCI_B_SPI_CLOCKSOURCE_SMCLK;
    param.clockSourceFrequency = UCS_getSMCLK();
    param.desiredSpiClock = SPICLK;
    param.msbFirst = USCI_B_SPI_MSB_FIRST;
    param.clockPhase = USCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    param.clockPolarity = USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
    returnValue =  USCI_B_SPI_initMaster(USCI_B0_BASE, &param);

    if (STATUS_FAIL == returnValue){
        return;
    }

    //Enable SPI module
    USCI_B_SPI_enable(USCI_B0_BASE);

    //Enable Receive interrupt
    USCI_B_SPI_clearInterrupt(USCI_B0_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);
    USCI_B_SPI_enableInterrupt(USCI_B0_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);

    //Now with SPI signals initialized, reset slave
    /*
    GPIO_setOutputLowOnPin(
        GPIO_PORT_P1,
        GPIO_PIN1
        );
        */

    return 0;
}
