/**
 * @file radio.c
 * @brief This file controls the radio hardware peripheral in both transmitting
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
unsigned char rxPacketStartedFlag = 0;      /**< A flag indicating that a new packet reception is to be started */
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

  //Set receiving flag to 1 indicating active receive mode
  receivingFlag = 1;
}

void ReceiveOff(void)
{
  RF1AIE &= ~BIT9;                          // Disable RX interrupts
  RF1AIFG &= ~BIT9;                         // Clear pending IFG
  RF1AIES &= ~BIT9;                         // Switch back to to sync word

  //Set the radio module into IDLE mode
  Strobe(RF_SIDLE);

  //Set recieving flag to 0 indicate disabling of main logic receive functionality
  receivingFlag = 0;

  //Stop the receive logic timer incase it was not stopped. (Should be stopped on completion of received packet).
  StopRadioRxTimer();
}


void ReceivePacket(void)
{
  //This should only be reinitialized when needed at the start of a NEW packet!
  rxBytesLeft = PACKET_LEN + 2;// Set maximum packet leng + 2 for appended bytes
  rxPosition = 0;
  packetReceivedFlag = 0;
  rxintervalcnt = 0;

  //Wait for bytes to fill in RX FIFO (MCLK = 12MHz, 2.8ms)
  __delay_cycles(33600);

  //Start the packet receiver timer
  StartRadioRxTimer();
}

void TransmitPacket(unsigned char len)
{
    //Reset logic variables
    txBytesLeft = len;
    txPosition = 0;
    packetTransmitFlag = 0;
    transmittingFlag = 1;

    //Setup CC1190
    RfLowNoiseAmplifierDisable();
    RfHighGainModeEnable();
    RfPowerAmplifierEnable();

    //Set radio module into transmit mode
    Strobe( RF_STX );                         // Strobe STX

    //Start the transmit packet timer
    StartRadioTxTimer();

}

//------------------------------------------------------------------------------
//  void pktRxHandler(void)
//
//  DESCRIPTION:
//      This function is called every time a timer interrupt occurs. The
//      function starts by retrieving the status byte. Every time the status
//      byte indicates that there are available bytes in the RX FIFO, bytes are
//      read from the RX FIFO and written to RxBuffer. This is done until the
//      whole packet is received. If the status byte indicates that there has
//      been an RX FIFO overflow the RX FIFO is flushed. Please see the
//      EM430F6137RF900 RF Examples User Manual for a flow chart describing this
//      function.
//------------------------------------------------------------------------------
void pktRxHandler(void) {
  unsigned char RxStatus; /**< Variable that stores the radio module receiver status byte */
  unsigned char bytesInFifo; /**< Stores the number of bytes that can be retrieved from the FIFO */
  unsigned char bytestoget;  /** Stores the number of bytes to remove from the FIFO. Per user guide errors will occurs if final byte retrieve if the last byte is not the final by of a packet */

  // Get the radio receiver state
  RxStatus = Strobe(RF_SNOP);

  switch(RxStatus & CC430_STATE_MASK)
  {
    case CC430_STATE_IDLE:
        __no_operation();
        break;
    case CC430_STATE_RX_OVERFLOW:
        //Flush RX FIFO
        Strobe(RF_SIDLE);
        Strobe(RF_SFRX);

        //Reset the packet started variable flag
        rxPacketStartedFlag = 0;

        //Turn the receiver off
        ReceiveOff();

        //Flush the receiver FIFO
        FlushReceiveFifo();
        break;
    case CC430_STATE_TX_UNDERFLOW:
        __no_operation();
        break;
    case CC430_STATE_RX:
      // Save number of bytes in FIFO either as the FIFO size or if smaller the remaining bytes needed for the current packet
      bytesInFifo = MIN(rxBytesLeft, RxStatus & CC430_FIFO_BYTES_AVAILABLE_MASK);
      if (bytesInFifo)
      {
        //Update how many bytes are left to be received
          if(rxBytesLeft - bytesInFifo == 0){
              //Final bytes of packet are in FIFO, OK to remove ALL bytes
              __no_operation();
              rxBytesLeft -= bytesInFifo;
              bytestoget = bytesInFifo;
          }
          else{
              //Final bytes of the packet are NOT received, leave margin of bytes in FIFO to avoid error (see users guide)
              rxBytesLeft -= (bytesInFifo-3);
              bytestoget = bytesInFifo-3;
              __no_operation();
          }

        rxintervalcnt++;

        if(bytesInFifo<=rxBytesLeft){
            //If not the final bytes to retrieve from the FIFO get just the determied bytecount
            while (bytestoget) {
              RxBuffer[rxPosition] = ReadSingleReg(RXFIFO);
              rxPosition++;
              bytestoget--;
            }

        }
        else{
            //Get all bytes from FIFO @TODO Is this needed anymore? seems redundant with the above...
            while (bytestoget--) {
              RxBuffer[rxPosition] = ReadSingleReg(RXFIFO);
              rxPosition++;
            }
        }

        if (!rxBytesLeft){
            //No bytes are left, final bytes of packet has been received. Reset variables and enable flags as needed
            packetReceivedFlag = 1;
            receivingFlag = 0;
            rxPosition = 0;
            rxPacketStartedFlag = 0;

            //Turn the receiver OFF and flush  (just incase)
            ReceiveOff();
            FlushReceiveFifo();

            //Stop receiver timer
            StopRadioRxTimer();

        }
      }
      break;
    default:
        //If no receiver state matches there is an error or packet is completed. @TODO I think I removed the IDLE case from causing this default so this would only be called in ERROR?
        if(!packetReceivedFlag)
        {
        packetReceivedFlag = 1;
        }

        //Reset global receiver variables
        rxPacketStartedFlag = 0;
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

    //Get the radio module transmitter state
    TxStatus = Strobe(RF_SNOP);

    switch (TxStatus & CC430_STATE_MASK) {
        case CC430_STATE_TX:
            //Save into variable the current packet bytes left or current FIFO size, whichever is smaller.
            if (freeSpaceInFifo = MIN(txBytesLeft, TxStatus & CC430_FIFO_BYTES_AVAILABLE_MASK))
            {
              txBytesLeft -= freeSpaceInFifo;
              while(freeSpaceInFifo--)
              {
                WriteSingleReg(TXFIFO, TxBuffer[txPosition]);
                txPosition++;
              }
              if(!txBytesLeft)
              {
                //Enable interrupt for the TX END-OF-PACKET flag
                RF1AIES |= BIT9;      // End-of-packet TX interrupt
                RF1AIFG &= ~BIT9;     // clear RFIFG9

                //Wait for TX END-OF-PACKET interrupt flag
                while(!(RF1AIFG & BIT9)); // poll RFIFG9 for TX end-of-packet

                //Disable interrupt for the TX END-OF-PACKET flag
                RF1AIES &= ~BIT9;      // End-of-packet TX interrupt
                RF1AIFG &= ~BIT9;     // clear RFIFG9

                //Reset transmitter logic variables
                transmittingFlag = 0;
                packetTransmitFlag = 1;

                //Stop transmitter packet timer
                StopRadioTxTimer();

                //Flush receiver FIFO (Just incase) @TODO Is this really needed? I forget why I placed this here but probably in error.
                FlushReceiveFifo();

                //Put radio module into receive mode
                ReceiveOn();
              }
            }
            break;

        case CC430_STATE_TX_UNDERFLOW:
            Strobe(RF_SFTX);  // Flush the TX FIFO

            __no_operation();
            // No break here!
        default:
            //If no radio module state matches there must be an error #TODO I've added a catch for IDLE so this is only an error state, for now.
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
    //Turn receiver OFF
    ReceiveOff();

    //Update the cc430 radio module hardware packet length register is correct
    changeRfPacketLength(len);

    //Set global TxBuffer pointer to first index of the data array to be transmitted
    TxBuffer = data;

    //Initialize the transmission of the intended packet
    TransmitPacket(len);
} // TransmitData

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
    // If RX sync word received
    if(!(RF1AIES & BIT9))
        {
        //New packet is arriving, start flag to initiate receiver logic to receive packet
        incomingpacketflag = 1;
          __no_operation();
        }
        else while(1);                // trap, this is an error state @TODO Is this needed???

}

void radiomainloop(void){
    if(receivingFlag)
        {
        //If a new packet has already been started to be received @TODO Is this still useful, I think I put it here for debugging.
        if(rxPacketStartedFlag){
            __no_operation(); // Nothing to do, let ISR's in timer rx packet
        }

        //If a new packet is to be actively received
        if(incomingpacketflag){
            //Setup new packet
            incomingpacketflag = 0;
            ReceivePacket();
            rxPacketStartedFlag = 1;
        }
    }

    //Safety check to avoid a "deaf" radio state. If not transmitting or receiving enable the receiver mode
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
    //Counter is below the setpoint to initiate a test transmission, iterate the counter.
    if(txtestdatatimercnt<1){
        txtestdatatimercnt++;
    }
    //If the counter has reached the intended count to initiate a transmission then  transmit
    else{
        txtestdataflag = 1;
        txtestdatatimercnt = 0;
    }
}

void FlushReceiveFifo(void){
    Strobe(RF_SIDLE);
    Strobe(RF_SFRX);
}
