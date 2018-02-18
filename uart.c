/*
 * uart.c
 *
 *  Created on: Feb 17, 2018
 *      Author: KB1LQ
 */

#include "driverlib.h"
#include "uart.h"

unsigned char uarttransmitbyte(unsigned char databyte){
    USCI_A_UART_transmitData(
            USCI_A0_BASE,
            databyte);
}

