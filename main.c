/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

/**
 * @file main.c
 * @brief This source file contains the main functionality of the program.
 *
 * @author Brenton Salmi, KB1LQD
 *
 * @date 2/8/2018
 */

#include "driverlib.h"
#include "initializations/init_ucs.h" // @TODO Make this a project global initialization directory for the linker.
#include "initializations/init_spi.h" // @TODO Make this a project global initialization directory for the linker.
#include "initializations/init_gpio.h" // @TODO Make this a project global initialization directory for the linker.
#include "initializations/init_uart.h" // @TODO Make this a project global initialization directory for the linker.
#include "initializations/init_rf.h" // @TODO Make this a project global initialization directory for the linker.
#include "initializations/init_timer.h" // @TODO Make this a project global initialization directory for the linker.
#include "uart.h"
#include "radio.h"

/**
 * This function is the main function of the program. It contains an infinite
 * loop that iterates indefinitely.
 * @author Brenton Salmi, KB1LQD
 * @return Void
 * @date 2/8/2018
 */
void main (void)
{
    //Stop WDT
    WDT_A_hold(WDT_A_BASE);

    //Initialize UCS to 12MHz
    initialize_ucs();

    //Initialize SPI
    init_gpio_spi();
    init_spi();
    init_uart();
    init_radio();
    InitTimer();
    StartTestTimer();
    StartRadioMainTimer();
    ReceiveOn();


    // Enable global interrupt
    __bis_SR_register(GIE);

    //Test
    //uartselftest();
    //unsigned char txdata[5] = {0, 1, 2, 3, 4};

    //init_self_test_fifo();
    //fifo_selftest();

    changeRfPacketLength(50);
    changeRfPacketLength(253);
    __no_operation();

    CreateTestRadioData();
    TransmitTestRadioData();

    /*unsigned char i;
    for(i=0; i<100; i++){
        TransmitTestRadioData();
    }
    */


    // Infinite main loop
    while(1){


        //__delay_cycles(12000000);
        //TransmitTestRadioData();
        radiomainloop();

        radiotestdatamainloop();


        __no_operation();
    } // END Infinite main loop
}


/**
 * This function is the interrupt handler for the Unified Clock System (UCS).
 * It is only used if the clock(s) fail to initialize and fault, an infinite loop
 * exists to "trap"  the program counter.
 * @author Brenton Salmi, KB1LQD
 * @return Void
 * @date 2/8/2018
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=UNMI_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(UNMI_VECTOR)))
#endif
void NMI_ISR(void)
{
  do {
    // If it still can't clear the oscillator fault flags after the timeout,
    // trap and wait here.
    status = UCS_clearAllOscFlagsWithTimeout(1000);
    __no_operation();
  } while(status != 0);
}


/**
 * This is the USCI_B0 interrupt vector service routine.
 * @author Brenton Salmi, KB1LQD
 * @date 2/9/2018
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_B0_VECTOR)))
#endif
void USCI_B0_ISR (void)
{
    switch (__even_in_range(UCB0IV,4)){
        //Vector 2 - RXIFG
        case 2:
            //USCI_A0 TX buffer ready?
            while (!USCI_B_SPI_getInterruptStatus(USCI_B0_BASE,
                       USCI_B_SPI_TRANSMIT_INTERRUPT)) ;

            //Copy received byte buffer into global variable
            receiveData = USCI_B_SPI_receiveData(USCI_B0_BASE);

            //Delay between transmissions for slave to process information
            __delay_cycles(40);


            break;
        default: break;
    }
}

//******************************************************************************
//
//This is the USCI_A0 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A0_VECTOR)))
#endif
void USCI_A0_ISR (void)
{
    switch (__even_in_range(UCA0IV,4)){
        //Vector 2 - RXIFG
        case 2:
            uartreceivedData = USCI_A_UART_receiveData(USCI_A0_BASE);
            __no_operation();
            break;
        default: break;
    }
}

//******************************************************************************
//
//This is the CC1101 Radio interrupt vector service routine.
//
//******************************************************************************
#pragma vector=CC1101_VECTOR
__interrupt void CC1101_ISR(void)
{
  switch(__even_in_range(RF1AIV,32))        // Prioritizing Radio Core Interrupt
  {
    case  0: break;                         // No RF core interrupt pending
    case  2: break;                         // RFIFG0
    case  4: break;                         // RFIFG1
    case  6: break;                         // RFIFG2
    case  8: break;                         // RFIFG3
    case 10: break;                         // RFIFG4
    case 12: break;                         // RFIFG5
    case 14: break;                         // RFIFG6
    case 16: break;                         // RFIFG7
    case 18: break;                         // RFIFG8
    case 20:                                // RFIFG9
        radioisr();
      break;
    case 22: break;                         // RFIFG10
    case 24: break;                         // RFIFG11
    case 26: break;                         // RFIFG12
    case 28: break;                         // RFIFG13
    case 30: break;                         // RFIFG14
    case 32: break;                         // RFIFG15
  }
}


//******************************************************************************
//
//This is the Timer0_A1 interrupt vector service routine.
//
//******************************************************************************
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
{
  switch(__even_in_range(TA0IV,14))
  {
    case 0:  break;
    case 2:
        radiotimerisr();
      break;
    case 4:
        __no_operation();
        RadioTestTimerIsr();
        break;                         // CCR2 not used
    case 6:
        __no_operation();
        break;                         // Reserved not used
    case 8:  break;                         // Reserved not used
    case 10: break;                         // Reserved not used
    case 12: break;                         // Reserved not used
    case 14: break;                         // Overflow not used
  }
}
