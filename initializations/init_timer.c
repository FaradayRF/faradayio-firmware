/*
 * init_timer.c
 *
 *  Created on: Feb 18, 2018
 *      Author: KB1LQ
 */

#include "cc430x613x.h"
#include "init_timer.h"
#include "../HAL/hal_UCS.h"

void InitTimer(void)
{
  P5SEL |= 0x03;                            // Set xtal pins
  LFXT_Start(XT1DRIVE_0);

  TA0CCR1  = RX_TIMER_PERIOD;               // x cycles * 1/32768 = y us
  TA0CCTL1 = CCIE;                          // Enable interrupts
  TA0CTL   = TASSEL__ACLK + TACLR;          // ACLK source
}

