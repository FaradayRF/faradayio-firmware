/**
 * @file cc1190.h
 * @brief This file controls the radio hardware of the CC1190 transmitter amplifier
 * and reciever low noise amplifier on Faraday.
 * @author Brenton Salmi, KB1LQD
 *
 * @date 2/25/2018
 */

#ifndef CC1190_H_
#define CC1190_H_


/**
 * CC430 CC1190 transmitter RF amplifier GPIO port and pin
 */
#define PA_ENABLE GPIO_PIN5

/**
 * CC430 CC1190 reciever RF low noise amplifier GPIO port and pin
 */
#define LNA_ENABLE GPIO_PIN6

/**
 * CC430 CC1190 high gain mode GPIO port and pin
 */
#define HGM_SELECT GPIO_PIN7


/**
 * Enable the RF transmitter power amplifier of the CC1190.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 3/5/2018
 */
void RfPowerAmplifierEnable(void);

/**
 * Disable the RF transmitter power amplifier of the CC1190.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 3/5/2018
 */
void RfPowerAmplifierDisable(void);

/**
 * Enable the RF receiver low noise amplifier of the CC1190.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 3/5/2018
 */
void RfLowNoiseAmplifierEnable(void);

/**
 * Disable the RF receiver low noise amplifier of the CC1190.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 3/5/2018
 */
void RfLowNoiseAmplifierDisable(void);

/**
 * Enable the CC1190 high gain mode (transmit & receive).
 *
 * @author Brenton Salmi, KB1LQD
 * @date 3/5/2018
 */
void RfHighGainModeEnable(void);

/**
 * Disable the CC1190 high gain mode (transmit & receive).
 *
 * @author Brenton Salmi, KB1LQD
 * @date 3/5/2018
 */
void RfHighGainModeDisable(void);


#endif /* CC1190_H_ */
