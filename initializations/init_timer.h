/*
 * init_timer.h
 *
 *  Created on: Feb 18, 2018
 *      Author: KB1LQ
 */

#ifndef INITIALIZATIONS_INIT_TIMER_H_
#define INITIALIZATIONS_INIT_TIMER_H_

/**
 * Timer period configuration setting for receive operation. //Why is this not in radio header?
 */
#define  RX_TIMER_PERIOD    (85)

/**
 * Timer period configuration setting for transmit operation. //Why is this not in radio header?
 */
#define  TX_TIMER_PERIOD    (40)

/**
 * This Initializes the timer perhiperal as needed by the radio transmit and receive operations.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 2/25/2018
 */
void InitTimer(void);


#endif /* INITIALIZATIONS_INIT_TIMER_H_ */
