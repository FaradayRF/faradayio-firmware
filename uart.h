/*
 * uart.h
 *
 *  Created on: Feb 17, 2018
 *      Author: KB1LQ
 */

#ifndef UART_H_
#define UART_H_

extern uint8_t uartreceivedData;

unsigned char uarttransmitbyte(unsigned char databyte);
unsigned char uartselftest(void);

#endif /* UART_H_ */
