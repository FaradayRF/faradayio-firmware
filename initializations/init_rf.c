/**
 * @file init_rf.c
 * @brief This header file contains initialization functions to enable
 * the radio hardware of the CC430 for use on Faraday.
 *
 * @author Brenton Salmi, KB1LQD
 *
 * @date 2/18/2018
 */

#include "init_rf.h"

unsigned char init_radio(void){
    // Increase PMMCOREV level to 2 for proper radio operation
    SetVCore(2);
}

