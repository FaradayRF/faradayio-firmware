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

#include "cc430x613x.h"
#include "../HAL/RF1A.h"
#include "../HAL/hal_PMM.h"
#include "../HAL/hal_UCS.h"

unsigned char init_radio(void);

#endif /* INITIALIZATIONS_INIT_RF_H_ */
