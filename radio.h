/*
 * radio.h
 *
 *  Created on: Feb 18, 2018
 *      Author: KB1LQ
 */

#ifndef RADIO_H_
#define RADIO_H_

#define  PACKET_LEN         (100)       // PACKET_LEN > FIFO Size
#define  RSSI_IDX           (PACKET_LEN+1)  // Index of appended RSSI
#define  CRC_LQI_IDX        (PACKET_LEN+2)  // Index of appended LQI, checksum
#define  CRC_OK             (BIT7)          // CRC_OK bit
#define  PATABLE_VAL        (0x51)          // 0 dBm output
#define  RX_TIMER_PERIOD    (85)
#define  TX_TIMER_PERIOD    (40)

#define CC430_STATE_TX                   (0x20)
#define CC430_STATE_IDLE                 (0x00)
#define CC430_STATE_TX_UNDERFLOW         (0x70)
#define CC430_STATE_MASK                 (0x70)
#define CC430_FIFO_BYTES_AVAILABLE_MASK  (0x0F)
#define CC430_STATE_RX                   (0x10)
#define CC430_STATE_RX_OVERFLOW          (0x60)

#define MIN(n,m) (((n) < (m)) ? (n) : (m))

void ReceiveOn(void);
void ReceiveOff(void);
void ReceivePacket(void);
void TransmitPacket(void);
void pktRxHandler(void);
void pktTxHandler(void);
unsigned char TransmitData(unsigned char *data);

#endif /* RADIO_H_ */
