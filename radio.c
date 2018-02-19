/*
 * radio.c
 *
 *  Created on: Feb 18, 2018
 *      Author: KB1LQ
 */

#include "radio.h"
#include "cc430x613x.h"
#include "HAL/RF1A.h"
void ReceiveOn(void)
{
  RF1AIES &= ~BIT9;
  RF1AIFG = 0;                              // Clear pending RFIFG interrupts
  RF1AIE  |= BIT9;                          // Enable the sync word received interrupt

  // Radio is in IDLE following a TX, so strobe SRX to enter Receive Mode
  Strobe( RF_SRX );

  __no_operation();
}

void ReceiveOff(void)
{
  RF1AIE &= ~BIT9;                          // Disable RX interrupts
  RF1AIFG &= ~BIT9;                         // Clear pending IFG
  RF1AIES &= ~BIT9;                         // Switch back to to sync word

  // It is possible that ReceiveOff is called while radio is receiving a packet.
  // Therefore, it is necessary to flush the RX FIFO after issuing IDLE strobe
  // such that the RXFIFO is empty prior to receiving a packet.
  Strobe(RF_SIDLE);
  Strobe(RF_SFRX);
}


