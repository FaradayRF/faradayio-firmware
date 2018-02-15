/*
 * init_spi.h
 *
 *  Created on: Feb 9, 2018
 *      Author: KB1LQ
 */

#ifndef INITIALIZATIONS_INIT_SPI_H_
#define INITIALIZATIONS_INIT_SPI_H_

/**
 * Transmit data byte value
 */
extern uint8_t transmitData;

/**
 * Receive data byte value
 */
extern uint8_t receiveData;

/**
 * Return byte data byte value
 */
extern uint8_t returnValue;

/**
 * This function initializes the CC430 SPI hardware on the USCI B peripheral
 * hardware in SPI mode.
 * @author Brenton Salmi, KB1LQD
 * @return False
 * @date 2/9/2018
 */
unsigned char init_spi(void);



#endif /* INITIALIZATIONS_INIT_SPI_H_ */
