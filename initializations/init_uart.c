/*
 * init_uart.c
 *
 *  Created on: Feb 17, 2018
 *      Author: KB1LQ
 */


#include "driverlib.h"

//*****************************************************************************
//
//Select Baud rate
//
//*****************************************************************************
#define BAUD_RATE                               9600
//*****************************************************************************
//
//Initialize received data
//
//*****************************************************************************
uint8_t uartreceivedData = 0x00;
//*****************************************************************************
//
//Initialize trasnmit data
//
//*****************************************************************************
uint8_t uarttransmitData = 0x00;

uint8_t check = 0;

unsigned char init_uart(void){
    //Baudrate = 9600, clock freq = 1.048MHz
    //UCBRx = 109, UCBRFx = 0, UCBRSx = 2, UCOS16 = 0
    USCI_A_UART_initParam param = {0};
    param.selectClockSource = USCI_A_UART_CLOCKSOURCE_SMCLK;
    param.clockPrescalar = 104;
    param.firstModReg = 0;
    param.secondModReg = 0;
    param.parity = USCI_A_UART_NO_PARITY;
    param.msborLsbFirst = USCI_A_UART_LSB_FIRST;
    param.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
    param.uartMode = USCI_A_UART_MODE;
    param.overSampling = USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;

    if (STATUS_FAIL == USCI_A_UART_init(USCI_A0_BASE, &param)){
        return;
    }

    //Enable UART module for operation
    USCI_A_UART_enable(USCI_A0_BASE);

    //Enable Receive Interrupt
      USCI_A_UART_clearInterrupt(USCI_A0_BASE,
        USCI_A_UART_RECEIVE_INTERRUPT);
    USCI_A_UART_enableInterrupt(USCI_A0_BASE,
        USCI_A_UART_RECEIVE_INTERRUPT);
}
