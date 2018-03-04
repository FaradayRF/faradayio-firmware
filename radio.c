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

unsigned char packetReceived;
unsigned char packetTransmit;

unsigned char txBytesLeft = PACKET_LEN;           // +1 for length byte
unsigned char txPosition = 0;
unsigned char rxBytesLeft = PACKET_LEN+2;         // +2 for status bytes
unsigned char rxPosition = 0;
unsigned char lengthByteRead = 0;
unsigned char rxPacketStarted = 0;

unsigned char RxBufferLength = 0;
unsigned char TxBufferLength = 0;
unsigned char * _p_Buffer = 0;
unsigned char buttonPressed = 0;
unsigned int i = 0;
unsigned char fifofillcount = 0;

unsigned char transmitting = 0;
unsigned char receiving = 0;

unsigned char RxBuffer[PACKET_LEN+2] = {0};
unsigned char DebugBuffer[PACKET_LEN+2] = {0};
/*
unsigned char TxBuffer[PACKET_LEN]= {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99,
};
*/

unsigned char * TxBuffer;

unsigned char data[253];
struct RfTxBuffer RfTxBuffer1;

volatile unsigned char test2;

unsigned char txtestdataflag;
unsigned int txtestdatatimercnt;
unsigned char incomingpacketflag;

unsigned char rxintervalcnt;

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

  receiving = 1;
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

  receiving = 0;
  StopRadioRxTimer();
}


void ReceivePacket(void)
{
  //This should only be reinitialized when needed at the start of a NEW packet!
  rxBytesLeft = PACKET_LEN + 2;// Set maximum packet leng + 2 for appended bytes
  rxPosition = 0;
  packetReceived = 0;

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
  packetTransmit = 0;
  transmitting = 1;
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

  // Which state?
  RxStatus = Strobe(RF_SNOP);

  switch(RxStatus & CC430_STATE_MASK)
  {
    case CC430_STATE_RX_OVERFLOW:
        //Flush RX FIFO
        Strobe(RF_SIDLE);
        Strobe(RF_SFRX);
        rxPacketStarted = 0;
        ReceiveOff();
        break;
    case CC430_STATE_TX_UNDERFLOW:
        __no_operation();
        break;
    case CC430_STATE_RX:
      // If there's anything in the RX FIFO....
      if (bytesInFifo = MIN(rxBytesLeft, RxStatus & CC430_FIFO_BYTES_AVAILABLE_MASK))
      {
        // Update how many bytes are left to be received
        rxBytesLeft -= bytesInFifo;

        DebugBuffer[rxintervalcnt] = bytesInFifo;
        rxintervalcnt++;

        // Read from RX FIFO and store the data in rxBuffer

        if(bytesInFifo<=rxBytesLeft){
            while (bytesInFifo>3) {
              RxBuffer[rxPosition] = ReadSingleReg(RXFIFO);
              rxPosition++;
              bytesInFifo--;
            }

        }
        else{
            while (bytesInFifo--) {
              RxBuffer[rxPosition] = ReadSingleReg(RXFIFO);
              rxPosition++;
            }
        }

        if (!rxBytesLeft){
            packetReceived = 1;
            receiving = 0;
            lengthByteRead = 0;
            rxPosition = 0;
            rxPacketStarted = 0;
            ReceiveOff();
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
      receiving = 0;
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
                transmitting = 0;
                packetTransmit = 1;
                StopRadioTxTimer();
              }
            }
            break;

        case CC430_STATE_TX_UNDERFLOW:
            Strobe(RF_SFTX);  // Flush the TX FIFO

            __no_operation();
            // No break here!
        default:
            if(!packetTransmit)
              packetTransmit = 1;

            if (transmitting) {
                if ((TxStatus & CC430_STATE_MASK) == CC430_STATE_IDLE) {
                  transmitting = 0;
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
    if(receiving)
    {
      TA0CCR1 += RX_TIMER_PERIOD;                  // 16 cycles * 1/32768 = ~500 us

      pktRxHandler();

      if(packetReceived)
          __no_operation();
          packetReceived = 0;
        //__bic_SR_register_on_exit(LPM3_bits);
    }

    else if(transmitting)
    {
      TA0CCR1 += TX_TIMER_PERIOD;                  // 16 cycles * 1/32768 = ~500 us

      pktTxHandler();

      if(packetTransmit)
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
    if(receiving)
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

    if(!transmitting & !receiving){
        ReceiveOn();
    }
}

void CreateTestRadioData(void){
    unsigned char i;

    for(i=0; i<253; i++){
        data[i]= i;
    }

    RfTxBuffer1.len = 253;
    RfTxBuffer1.data = data;
}

void TransmitTestRadioData(void){
    if(!transmitting){
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
