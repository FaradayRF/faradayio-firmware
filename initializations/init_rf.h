/**
 * @file init_rf.h
 * @brief This header file contains initialization functions to enable
 * the radio hardware of the CC430 for use on Faraday.
 *
 * @author Brenton Salmi, KB1LQD
 *
 * @date 2/18/2018
 */

#ifndef INITIALIZATIONS_INIT_RF_H_
#define INITIALIZATIONS_INIT_RF_H_

/**
 * This is the main high level function to call to initialize the radio module.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 3/5/2018
 */
unsigned char init_radio(void);

/**
 * This function allows modification of only the internal RfSettings register value that is the
 * Packet Length register. This register controls the radio module hardware packet functionality
 * packet length register.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 3/5/2018
 */
void changeRfPacketLength(unsigned char length);

/**
 * Initiates a manual calibration of the radion module PLL.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 3/5/2018
 */
void radio_manual_calibration_idle(void);

/**
 * Puts the radio module into IDLE state.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 3/5/2018
 */
void radio_manual_idle(void);

#endif /* INITIALIZATIONS_INIT_RF_H_ */
