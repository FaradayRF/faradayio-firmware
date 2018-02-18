/**
 * @file uart.c
 * @brief This header file contains high level abstraction functions
 * of the UART of the CC430 for use on Faraday.
 *
 * @author Brenton Salmi, KB1LQD
 *
 * @date 2/17/2018
 */

#include "driverlib.h"
#include "uart.h"


unsigned char uarttransmitbyte(unsigned char databyte){
    USCI_A_UART_transmitData(
            USCI_A0_BASE,
            databyte);

    return 0;
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

    return 0;
}

