/**
 * @file init_gpio.h
 * @brief This header file contains all the required high level functions used
 * to initialize the GPIO ports of the CC430 for use on Faraday.
 *
 * @author Brenton Salmi, KB1LQD
 *
 * @date 2/17/2018
 */

#ifndef INITIALIZATIONS_INIT_GPIO_H_
#define INITIALIZATIONS_INIT_GPIO_H_

/**
 * This function initializes the CC430 GPIO for use with the USCI B peripheral
 * hardware in SPI mode.
 * @author Brenton Salmi, KB1LQD
 * @return False
 * @date 2/9/2018
 */
unsigned char init_gpio_spi(void);



#endif /* INITIALIZATIONS_INIT_GPIO_H_ */
