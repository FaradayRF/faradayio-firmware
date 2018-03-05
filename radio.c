/**
 * @file radio.c
 * @brief This file controls the radio hardware perhiperal in both transmitting
 * and receiving modes.
 *
 * @author Brenton Salmi, KB1LQD
 *
 * @date 2/25/2018
 */

#include "radio.h"
#include "cc430x613x.h"
#include "HAL/RF1A.h"
#include "cc1190.h"
#include "initializations/init_rf.h"
#include "initializations/init_timer.h"

/** @name Radio Variables
*
*   Variables used during transmitting, receiving, and debug of
*   the CC430 radio module on Faraday.
*
*
@{**/
unsigned char packetReceivedFlag;            /**< Flag indicating that a new packet has been received. */
unsigned char packetTransmitFlag;           /**< Flag indicating that a packet is being transmitted, set when initialized new transmission. */
unsigned char txBytesLeft = PACKET_LEN;     /**< Variable that holds the current count of bytes left to transmit in the current packet. +1 for length byte */
unsigned char txPosition = 0;               /**< Variable that holds the current buffer index position respective to the transmit buffer. */
unsigned char rxBytesLeft = PACKET_LEN+2;   /**< Variable that holds the current count of bytes left to receive in the current packet. +2 for status bytes */
unsigned char rxPosition = 0;               /**< Variable that holds the current buffer index position respective to the receive buffer. */
unsigned char transmittingFlag = 0;         /**< Flag indicating that the radio module control algorithm logical state is in transmitting mode. */
unsigned char receivingFlag = 0;            /**< Flag indicating that the radio module control algorithm logical state is in receiving mode. */
unsigned char RxBuffer[PACKET_LEN+2] = {0}; /**< Array that is used to hold the bytes received from the radio module. */
unsigned char * TxBuffer;                   /**< A global pointer used to point to the array bytes that is to be transmitted. */
unsigned char testdataarray[253];           /**< A test array used to hold fake data that is to be transmitted. */
struct RfTxBuffer RfTxBuffer1;              /**< A struct that holds information about expected packet length and pointer to the array to save received data into. */
unsigned char txtestdataflag;               /**< A flag that indicates the main test transmission loop logic should initiate a test data transmission. */
unsigned int txtestdatatimercnt;            /**< A counter used to count the interrations test transmitter timer, it is used to count to the predetermine trigger point creating a period interval.*/
unsigned char incomingpacketflag;           /**< A flag indicating that a new packet is being received. */
unsigned char rxintervalcnt;                /**< A test counter variable used to check how many iterations of getting from the receive FIFO were needed to receive a packet. */
/** @}*/

void ReceiveOn(void)
{

  //Setup CC1190
  RfHighGainModeEnable();
  RfPowerAmplifierDisable();
  RfLowNoiseAmplifierEnable();


  //Receive Routine
  RF1AIES &= ~BIT9;
  RF1AIFG = 0;                              // Clear pending RFIFG interrupts
  RF1AIE  |= BIT9;                          // Enable the sync word received interrupt

  // Radio is in IDLE following a TX, so strobe SRX to enter Receive Mode
  Strobe(RF_SIDLE);
  Strobe( RF_SRX );

  receivingFlag = 1;
}

void ReceiveOff(void)
{
  RF1AIE &= ~BIT9;                          // Disable RX interrupts
  RF1AIFG &= ~BIT9;                         // Clear pending IFG
  RF1AIES &= ~BIT9;                         // Switch back to to sync word

  // It is possible that ReceiveOff is called while radio is receiving a packet.
  // Therefore, it is necessary to flush the RX FIFO after issuing IDLE strobe
  // such that the RXFIFO is empty prior to receivingFlag a packet.
  Strobe(RF_SIDLE);
  //Strobe(RF_SFRX);

  receivingFlag = 0;
  StopRadioRxTimer();
}


void ReceivePacket(void)
{
  //This should only be reinitialized when needed at the start of a NEW packet!
  rxBytesLeft = PACKET_LEN + 2;// Set maximum packet leng + 2 for appended bytes
  rxPosition = 0;
  packetReceivedFlag = 0;

  __delay_cycles(33600);                     // Wait for bytes to fill in RX FIFO (MCLK = 12MHz, 2.8ms)

  rxintervalcnt = 0;
  StartRadioRxTimer();
}

void TransmitPacket(unsigned char len)
{

  //Setup CC1190
  RfLowNoiseAmplifierDisable();
  RfHighGainModeEnable();
  RfPowerAmplifierEnable();

  //Transmit routine
  txBytesLeft = len;
  txPosition = 0;
  packetTransmitFlag = 0;
  transmittingFlag = 1;
  Strobe( RF_STX );                         // Strobe STX
  StartRadioTxTimer();

}

//------------------------------------------------------------------------------
//  void pktRxHandler(void)
//
//  DESCRIPTION:
//      This function is called every time a timer interrupt occurs. The
//      function starts by retreiving the status byte. Every time the status
//      byte indicates that there are available bytes in the RX FIFO, bytes are
//      read from the RX FIFO and written to RxBuffer. This is done until the
//      whole packet is received. If the status byte indicates that there has
//      been an RX FIFO overflow the RX FIFO is flushed. Please see the
//      EM430F6137RF900 RF Examples User Manual for a flow chart describing this
//      function.
//------------------------------------------------------------------------------
void pktRxHandler(void) {
  unsigned char RxStatus;
  unsigned char bytesInFifo;
  unsigned char bytestoget;

  // Which state?
  RxStatus = Strobe(RF_SNOP);

  switch(RxStatus & CC430_STATE_MASK)
  {
    case CC430_STATE_IDLE:
        __no_operation();
        //ReceiveOn();
        break;
    case CC430_STATE_RX_OVERFLOW:
        //Flush RX FIFO
        Strobe(RF_SIDLE);
        Strobe(RF_SFRX);
        rxPacketStarted = 0;
        ReceiveOff();
        FlushReceiveFifo();
        break;
    case CC430_STATE_TX_UNDERFLOW:
        __no_operation();
        break;
    case CC430_STATE_RX:
      // If there's anything in the RX FIFO....
      bytesInFifo = MIN(rxBytesLeft, RxStatus & CC430_FIFO_BYTES_AVAILABLE_MASK);
      if (bytesInFifo)
      {
        // Update how many bytes are left to be received
          if(rxBytesLeft - bytesInFifo == 0){
              __no_operation();
              rxBytesLeft -= bytesInFifo;
              bytestoget = bytesInFifo;
          }
          else{
              rxBytesLeft -= (bytesInFifo-3);
              bytestoget = bytesInFifo-3;
              __no_operation();
          }


        DebugBuffer[rxintervalcnt] = bytesInFifo;
        rxintervalcnt++;

        // Read from RX FIFO and store the testdataarray in rxBuffer

        if(bytesInFifo<=rxBytesLeft){
            while (bytestoget) {
              RxBuffer[rxPosition] = ReadSingleReg(RXFIFO);
              rxPosition++;
              bytestoget--;
            }

        }
        else{
            while (bytestoget--) {
              RxBuffer[rxPosition] = ReadSingleReg(RXFIFO);
              rxPosition++;
            }
        }

        if (!rxBytesLeft){
            packetReceived = 1;
            receivingFlag = 0;
            rxPosition = 0;
            rxPacketStarted = 0;
            ReceiveOff();
            FlushReceiveFifo();
            StopRadioRxTimer();

        }
      }
      break;
    default:
      if(!packetReceived)
      {
        packetReceived = 1;
      }

      rxPacketStarted = 0;
      rxBytesLeft = 0;
      receivingFlag = 0;
      break;
  }
} // pktRxHandler

//------------------------------------------------------------------------------
//  void pktTxHandler(void)
//
//  DESCRIPTION:
//      This function is called every time a timer interrupt occurs. The function starts
//      by getting the status byte. Every time the status byte indicates that there
//      is free space in the TX FIFO, bytes are taken from txBuffer and written to
//      the TX FIFO until the whole packet is written or the TXFIFO has underflowed.
//      See the EM430F6137RF900 RF Examples User Manual for a flow chart describing
//      this function.
//------------------------------------------------------------------------------
void pktTxHandler(void) {
    unsigned char freeSpaceInFifo;
    unsigned char TxStatus;

    // Which state?
    TxStatus = Strobe(RF_SNOP);

    switch (TxStatus & CC430_STATE_MASK) {
        case CC430_STATE_TX:
            // If there's anything to transfer..
            if (freeSpaceInFifo = MIN(txBytesLeft, TxStatus & CC430_FIFO_BYTES_AVAILABLE_MASK))
            {
              txBytesLeft -= freeSpaceInFifo;
              fifofillcount+=1;

              while(freeSpaceInFifo--)
              {

                WriteSingleReg(TXFIFO, TxBuffer[txPosition]);
                DebugBuffer[txPosition] = TxBuffer[txPosition];
                txPosition++;
              }

              if(!txBytesLeft)
              {
                RF1AIES |= BIT9;      // End-of-packet TX interrupt
                RF1AIFG &= ~BIT9;     // clear RFIFG9
                while(!(RF1AIFG & BIT9)); // poll RFIFG9 for TX end-of-packet
                RF1AIES &= ~BIT9;      // End-of-packet TX interrupt
                RF1AIFG &= ~BIT9;     // clear RFIFG9
                transmittingFlag = 0;
                packetTransmitFlag = 1;
                StopRadioTxTimer();
                FlushReceiveFifo();
                ReceiveOn();
              }
            }
            break;

        case CC430_STATE_TX_UNDERFLOW:
            Strobe(RF_SFTX);  // Flush the TX FIFO

            __no_operation();
            // No break here!
        default:
            if(!packetTransmitFlag)
              packetTransmitFlag = 1;

            if (transmittingFlag) {
                if ((TxStatus & CC430_STATE_MASK) == CC430_STATE_IDLE) {
                  transmittingFlag = 0;
                }
            }
        break;
    }
} // pktTxHandler

void TransmitData(unsigned char *data, unsigned char len){
    ReceiveOff();
    changeRfPacketLength(len);
    TxBuffer = data;
    TransmitPacket(len);
}

void radiotimerisr(void){
    if(receivingFlag)
    {
      TA0CCR1 += RX_TIMER_PERIOD;                  // 16 cycles * 1/32768 = ~500 us

      pktRxHandler();

      if(packetReceivedFlag){
          __no_operation();
          packetReceivedFlag = 0;
      }
    }

    else if(transmittingFlag)
    {
      TA0CCR1 += TX_TIMER_PERIOD;                  // 16 cycles * 1/32768 = ~500 us

      pktTxHandler();

      if(packetTransmitFlag)
          __no_operation();
    }
}

void radioisr(void){
    if(!(RF1AIES & BIT9))                 // RX sync word received
        {
        //receiving = 1;
        incomingpacketflag = 1;
          __no_operation();
        }
        else while(1);                // trap

}

void radiomainloop(void){
    if(receivingFlag)
        {
        if(rxPacketStarted){
            __no_operation(); // Nothing to do, let ISR's in timer rx packet
        }

        //if(!rxPacketStarted){
        if(incomingpacketflag){
            // Setup new packet
            incomingpacketflag = 0;
            ReceivePacket();
            rxPacketStarted = 1;
        }
    }

    if(!transmittingFlag & !receivingFlag){
        ReceiveOn();
    }
}

void CreateTestRadioData(void){
    unsigned char i;

    for(i=0; i<253; i++){
        testdataarray[i]= i;
    }

    RfTxBuffer1.len = 253;
    RfTxBuffer1.data = testdataarray;
}

void TransmitTestRadioData(void){
    if(!transmittingFlag){
    TransmitData((unsigned char *)RfTxBuffer1.data, 253);
    }
}

void radiotestdatamainloop(){
    if(txtestdataflag){
        TransmitTestRadioData();
        txtestdataflag = 0;
    }
}


void RadioTestTimerIsr(void){
    TA0CCR3  += TESTTIMERPERIOD;
    if(txtestdatatimercnt<1){
        txtestdatatimercnt++;
    }
    else{
        txtestdataflag = 1;
        txtestdatatimercnt = 0;
    }
}

void FlushReceiveFifo(void){
    Strobe(RF_SIDLE);
    Strobe(RF_SFRX);
}
