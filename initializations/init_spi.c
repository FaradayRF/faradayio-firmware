/*
 * init_spi.c
 *
 *  Created on: Feb 9, 2018
 *      Author: KB1LQ
 */

#include "driverlib.h"
#include "init_spi.h"

/**
 * Specify desired frequency of SPI communication
 */
#define SPICLK                          500000

uint8_t transmitData = 0x00;
uint8_t receiveData = 0x00;
uint8_t returnValue = 0x00;


