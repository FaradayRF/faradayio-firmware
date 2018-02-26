/**
 * @file uart.h
 * @brief This header file contains high level abstraction functions
 * of the UART of the CC430 for use on Faraday.
 *
 * @author Brenton Salmi, KB1LQD
 *
 * @date 2/17/2018
 */

#ifndef UART_H_
#define UART_H_

extern uint8_t uartreceivedData;

/**
 * Transmit a single byte over UART.
 * @author Brenton Salmi, KB1LQD
 * @return False
 * @date 2/17/2018
 *
 * @param databyte The byte intended to be transmitted
 */
unsigned char uarttransmitbyte(unsigned char databyte);

/**
 * A simple unit-test function to check uart operation.
 * @author Brenton Salmi, KB1LQD
 * @return False
 * @date 2/17/2018
 */
unsigned char uartselftest(void);

#endif /* UART_H_ */
