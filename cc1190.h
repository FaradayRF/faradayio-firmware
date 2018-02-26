/*
 * cc1190.h
 *
 *  Created on: Feb 21, 2018
 *      Author: KB1LQ
 */

#ifndef CC1190_H_
#define CC1190_H_


#define PA_ENABLE GPIO_PIN5
#define LNA_ENABLE GPIO_PIN6
#define HGM_SELECT GPIO_PIN7

void RfPowerAmplifierEnable(void);
void RfPowerAmplifierDisable(void);
void RfLowNoiseAmplifierEnable(void);
void RfLowNoiseAmplifierDisable(void);
void RfHighGainModeEnable(void);
void RfHighGainModeDisable(void);


#endif /* CC1190_H_ */
