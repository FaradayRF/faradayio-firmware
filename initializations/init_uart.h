/**
 * @file init_uart.h
 * @brief This header file contains initialization functions to enable
 * the UART hardware of the CC430 for use on Faraday.
 *
 * @author Brenton Salmi, KB1LQD
 *
 * @date 2/17/2018
 */

#ifndef INITIALIZATIONS_INIT_UART_H_
#define INITIALIZATIONS_INIT_UART_H_

/**
 * Global variable to hold the most recent received byte from the UART peripheral.
 */
extern uint8_t uartreceivedData;

/**
 * This function initializes the CC430 UART hardware to the desired baudrate.
 * @author Brenton Salmi, KB1LQD
 * @return False
 * @date 2/17/2018
 */
unsigned char init_uart(void);

#endif /* INITIALIZATIONS_INIT_UART_H_ */
