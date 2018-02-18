/**
 * @file init_spi.h
 * @brief This header file contains initialization functions to enable
 * the SPI hardware of the CC430 for use on Faraday.
 *
 * @author Brenton Salmi, KB1LQD
 *
 * @date 2/17/2018
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
