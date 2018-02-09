/*
 * init_gpio.c
 *
 *  Created on: Feb 9, 2018
 *      Author: KB1LQ
 */

#include "driverlib.h"
#include "init_gpio.h"

unsigned char init_gpio_spi(void){
    /**
     * SCLK = P1.4
     * MISO = P1.2 (Needs pullup?)
     * MOSI = P1.3 (Needs pullup?)
     * CS   = P5.5
     * HOLD = P5.6
     */

    //Set initial GPIO output values (Needed with ISCI B operation?)
    // SCLK
    GPIO_setOutputHighOnPin(
        GPIO_PORT_P1,
        GPIO_PIN4
        );

    // MOSI
    GPIO_setOutputHighOnPin(
        GPIO_PORT_P1,
        GPIO_PIN3
        );

    // CS
    GPIO_setOutputHighOnPin(
        GPIO_PORT_P5,
        GPIO_PIN5
        );

    // SCLK
    GPIO_setOutputHighOnPin(
        GPIO_PORT_P5,
        GPIO_PIN6
        );


    //option select input

    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1,
        GPIO_PIN2
        );


    return 0;
}
