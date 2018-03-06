/**
 * @file init_timer.c
 * @brief This file initializes and controls
 *
 * @author Brenton Salmi, KB1LQD
 *
 * @date 2/25/2018
 */

#include "cc430x613x.h"
#include "init_timer.h"
#include "../HAL/hal_UCS.h"

void InitTimer(void)
{
  P5SEL |= 0x03;                            // Set xtal pins
  LFXT_Start(XT1DRIVE_0);
  TA0CTL   = TASSEL__ACLK + TACLR;          // ACLK source
}

void StartRadioRxTimer(void){
    TA0CCR1   = RX_TIMER_PERIOD;              // x cycles * 1/32768 = y us
    TA0CCTL1  |= CCIE;
    TA0CTL    |= MC_2 + TACLR;          // ACLK source
}

void StopRadioRxTimer(void){
    TA0CCTL1 &= ~CCIE;
}

void StartRadioTxTimer(void){
    TA0CCR1   = TX_TIMER_PERIOD;              // x cycles * 1/32768 = y us
    TA0CCTL1  |= CCIE;
    TA0CTL    |= MC_2 + TACLR;          // ACLK source
}

void StopRadioTxTimer(void){
    TA0CCTL1 &= ~CCIE;
}

void StartTestTimer(void){
    TA0CCR2  = TESTTIMERPERIOD;               // x cycles * 1/32768 = y us
    TA0CCTL2 |= CCIE;                         // Enable interrupts
    TA0CTL   |= MC_2 + TACLR;          // ACLK source
}

void StopTestTimer(void){
    TA0CCTL2 &= ~CCIE;                         // Enable interrupts
}


void StartRadioMainTimer(void){
    TA0CCR3  = RADIOMAINTIMERPERIOD;               // x cycles * 1/32768 = y us
    TA0CCTL3 |= CCIE;                         // Enable interrupts
    TA0CTL   |= MC_2 + TACLR;          // ACLK source
}

void StopRadioMainTimer(void){
    TA0CCTL3 &= ~CCIE;                         // Enable interrupts
}
