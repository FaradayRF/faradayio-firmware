/**
 * @file init_ucs.h
 * @brief This header file contains all the required high level functions used
 * to initialize the Unified Clock System (UCS) of the CC430.
 *
 * @author Brenton Salmi, KB1LQD
 *
 * @date 2/8/2018
 */

#ifndef INITIALIZATIONS_INIT_UCS_H_
#define INITIALIZATIONS_INIT_UCS_H_

/**
 * Variable to store status of Oscillator fault flags
 */
uint16_t status;


/**
 * This function initializes the CC430 UCS peripheral to the predetermined
 * operational frequency(s) and configurations.
 * @author Brenton Salmi, KB1LQD
 * @return 0
 * @date 2/8/2018
 */
unsigned char initialize_ucs(void);


#endif /* INITIALIZATIONS_INIT_UCS_H_ */
