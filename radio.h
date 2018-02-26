/*
 * radio.h
 *
 *  Created on: Feb 18, 2018
 *      Author: KB1LQ
 */

#ifndef RADIO_H_
#define RADIO_H_

/**
 * PACKET_LEN > FIFO Size
 */
#define  PACKET_LEN         (100)

/**
 * Index of appended RSSI
 */
#define  RSSI_IDX           (PACKET_LEN+1)

/**
 * Index of appended LQI, checksum
 */
#define  CRC_LQI_IDX        (PACKET_LEN+2)

/**
 * CRC_OK bit
 */
#define  CRC_OK             (BIT7)

/**
 * 0 dBm output
 */
#define  PATABLE_VAL        (0x51)

/**
 * Timer ISR period setting during receive
 */
#define  RX_TIMER_PERIOD    (85)

/**
 * Timer ISR period setting during transmit
 */
#define  TX_TIMER_PERIOD    (40)

/**
 * Radio core transmit state bitmask
 */
#define CC430_STATE_TX                   (0x20)

/**
 * Radio core idle state
 */
#define CC430_STATE_IDLE                 (0x00)

/**
 * Radio core underflow state transmit
 */
#define CC430_STATE_TX_UNDERFLOW         (0x70)

/**
 * Radio core state mask
 */
#define CC430_STATE_MASK                 (0x70)

/**
 * Radio core fifo available bytes mask state bitmask
 */
#define CC430_FIFO_BYTES_AVAILABLE_MASK  (0x0F)

/**
 * Radio core receive state bitmask
 */
#define CC430_STATE_RX                   (0x10)

/**
 * Radio core receive overflow state bitmask
 */
#define CC430_STATE_RX_OVERFLOW          (0x60)

/**
 * minimum value function
 */
#define MIN(n,m) (((n) < (m)) ? (n) : (m))

void ReceiveOn(void);
void ReceiveOff(void);
void ReceivePacket(void);
void TransmitPacket(void);
void pktRxHandler(void);
void pktTxHandler(void);
void TransmitData(unsigned char *data);
void radiotimerisr(void);
void radioisr(void);
void radiomainloop(void);

#endif /* RADIO_H_ */
