/**
 * @file radio.h
 * @brief This file controls the radio hardware perhiperal in both transmitting
 * and receiving modes.
 *
 * @author Brenton Salmi, KB1LQD
 *
 * @date 2/25/2018
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

struct RfTxBuffer{
    unsigned char len;
    unsigned char * data;
};


/**
 * This function enables the CC430 radio module into active receive mode.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 2/25/2018
 */
void ReceiveOn(void);

/**
 * This function removes the CC430 radio module from active receive mode.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 2/25/2018
 */
void ReceiveOff(void);

/**
 * This function is called to start receiving a new packet. This should only
 * be executed at the start of brand new packet!
 *
 * @author Brenton Salmi, KB1LQD
 * @date 2/25/2018
 */
void ReceivePacket(void);

/**
 * This function configures the CC430 radio module into transmit mode and
 * also holds the sequence that transmits a packet.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 2/25/2018
 *
 * @param len Length of data to transmit
 */
void TransmitPacket(unsigned char len);

/**
 * This is the function used to actively query for new portions of a packet
 * being received. This is called after the ReceivePacket() function and
 * is mainly controlled by the timer.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 2/25/2018
 */
void pktRxHandler(void);

/**
 * This is the function used to actively sequence through the transmission of a
 * packet. It is mainly controlled by the timer ISR.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 2/25/2018
 */
void pktTxHandler(void);

/**
 * This is the high-level function that transmits a fixed array of data bytes.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 2/25/2018
 *
 * @param data Pointer to data to transmit
 * @param len Length of data to transmit
 */
void TransmitData(unsigned char *data, unsigned char len);

/**
 * Function that contains the logic sequences for transmit and receive operation
 * as needed by the associated timer interrupt.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 2/25/2018
 */
void radiotimerisr(void);

/**
 * * Function that contains the logic sequences for transmit and receive operation
 * and is called when the associated radio perhipheral interrupt occurs.
 * @author Brenton Salmi, KB1LQD
 * @date 2/25/2018
 */
void radioisr(void);

/**
 * The logic to perform during each iteration of the main infinite main loop for
 * both transmit and receive.
 *
 * @author Brenton Salmi, KB1LQD
 * @date 2/25/2018
 */
void radiomainloop(void);

void CreateTestRadioData(void);

void TransmitTestRadioData(void);

void radiotestdatamainloop(void);

void RadioTestTimerIsr(void);

#endif /* RADIO_H_ */
