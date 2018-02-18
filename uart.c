/*
 * uart.c
 *
 *  Created on: Feb 17, 2018
 *      Author: KB1LQ
 */

#include "driverlib.h"
#include "uart.h"

extern

unsigned char uarttransmitbyte(unsigned char databyte){
    USCI_A_UART_transmitData(
            USCI_A0_BASE,
            databyte);
}

unsigned char uartselftest(void){
    unsigned char i;
    for(i=0;i<255;i++){                      // Increment TX data
    // Load data onto buffer
    uarttransmitbyte(i);
    }

    while(uartreceivedData != 0x40){

    }
    __no_operation();
}

