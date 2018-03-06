/**
 * @file init_timer.h
 * @brief This file initializes and controls
 *
 * @author Brenton Salmi, KB1LQD
 *
 * @date 2/25/2018
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
 * Timer period configuration setting for test timer operation. Primeraly used for radio testing.
 */
#define  TESTTIMERPERIOD     (32767)

/**
 * Timer period configuration setting for the main loop housekeeping.
 */
#define  RADIOMAINTIMERPERIOD     (400)

/**
 * This Initializes the timer perhiperal as needed by the radio transmit and receive operations.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 2/25/2018
 */
void InitTimer(void);

/**
 * Starts the radio receiver packet timer interrupt.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 3/5/2018
 */
void StartRadioRxTimer(void);

/**
 * Stops the radio receiver packet timer interrupt.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 3/5/2018
 */
void StopRadioRxTimer(void);

/**
 * Starts the radio transmitter packet timer interrupt.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 3/5/2018
 */
void StartRadioTxTimer(void);

/**
 * Stops the radio transmitter packet timer interrupt.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 3/5/2018
 */
void StopRadioTxTimer(void);

/**
 * Starts the timer interrupt used to initiate periodic test functionality such as transmissions.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 3/5/2018
 */
void StartTestTimer(void);

/**
 * Stops the timer interrupt used to initiate periodic test functionality such as transmissions.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 3/5/2018
 */
void StopTestTimer(void);

/**
 * Starts the timer interrupt used to initiate periodic actions withing the main program loop.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 3/5/2018
 */
void StartRadioMainTimer(void);

/**
 * Stops the timer interrupt used to initiate periodic actions withing the main program loop.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 3/5/2018
 */
void StopRadioMainTimer(void);


#endif /* INITIALIZATIONS_INIT_TIMER_H_ */
