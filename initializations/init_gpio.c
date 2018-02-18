/**
 * @file init_gpio.c
 * @brief This header file contains all the required high level functions used
 * to initialize the GPIO ports of the CC430 for use on Faraday.
 *
 * @author Brenton Salmi, KB1LQD
 *
 * @date 2/17/2018
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

//    //Set initial GPIO output values (Needed with ISCI B operation?)
//    // SCLK
//    GPIO_setOutputHighOnPin(
//        GPIO_PORT_P1,
//        GPIO_PIN4
//        );
//
//    // MOSI
//    GPIO_setOutputHighOnPin(
//        GPIO_PORT_P1,
//        GPIO_PIN3
//        );

    // CS
    GPIO_setOutputHighOnPin(
        GPIO_PORT_P5,
        GPIO_PIN5
        );

//    // SCLK
//    GPIO_setOutputHighOnPin(
//        GPIO_PORT_P5,
//        GPIO_PIN6
//        );


    //option select input

    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1,
        GPIO_PIN2
        );

    //option select
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN3 + GPIO_PIN4
        );


    //P1.5,6 = USCI_A0 RXD/TXD UART
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1,
        GPIO_PIN5 + GPIO_PIN6
        );


    return 0;
}
