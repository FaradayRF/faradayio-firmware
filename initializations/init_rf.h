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

unsigned char init_radio(void);
void changeRfPacketLength(unsigned char length);
void radio_manual_calibration_idle(void);
void radio_manual_idle(void);

#endif /* INITIALIZATIONS_INIT_RF_H_ */
