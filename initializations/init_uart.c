/**
 * @file init_uart.h
 * @brief This header file contains initialization functions to enable
 * the UART hardware of the CC430 for use on Faraday.
 *
 * @author Brenton Salmi, KB1LQD
 *
 * @date 2/17/2018
 */


#include "driverlib.h"

/**
 * Desired baudrate
 */
#define BAUD_RATE                               115200


uint8_t uartreceivedData = 0x00;


unsigned char init_uart(void){
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
        return 0;
    }

    //Enable UART module for operation
    USCI_A_UART_enable(USCI_A0_BASE);

    //Enable Receive Interrupt
      USCI_A_UART_clearInterrupt(USCI_A0_BASE,
        USCI_A_UART_RECEIVE_INTERRUPT);
    USCI_A_UART_enableInterrupt(USCI_A0_BASE,
        USCI_A_UART_RECEIVE_INTERRUPT);

    return 0;
}
