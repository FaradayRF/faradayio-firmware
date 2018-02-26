/** @file FIFO.c
 *  @brief FIFO circular buffer implementation using CC430 RAM
 *
 *  These functions create a FIFO circular buffer using the onboard CC430 RAM. The circular buffers defined below are written as generic as
 *  possible to allow varying element lengths, counts, etc...
 *
 */

/* -- Includes -- */

/* standard includes */
#include "cc430f6137.h"
#include "fifo.h"


unsigned char put_fifo(fifo_state_machine *buffer_struct, volatile unsigned char *buffer, unsigned char *char_item){
    //Check if buffer is full!
    if((buffer_struct->inwaiting*buffer_struct->element_size) >= buffer_struct->buffer_size){
        //Set debut overflow bit
        buffer_struct->debug |= BIT0;

        //Return 0 to indicate failure (buffer full!)
        return 0;
    }

    //Buffer ready to accept new item
    else{
        unsigned char i;
        for(i=0;i<buffer_struct->element_size;i++){
            //Insert value into the FIFO ring buffer in next avaliable location (head)
            buffer[buffer_struct->head+i] = char_item[i];
        }

        //Increment head location of ring buffer, modulus allows easy "wrapping" (circle)
        buffer_struct->head = (buffer_struct->head+buffer_struct->element_size)%buffer_struct->buffer_size;

        //Increment the inwaiting variable to account for the new item in the FIFO
        buffer_struct->inwaiting++; //Increment the inwaiting count

        //Return 1 to indicate success
        //DEBUG - MAX INWAIT
        if(buffer_struct->inwaiting > buffer_struct->max_inwait){
            buffer_struct->max_inwait = buffer_struct->inwaiting;
        }

        return 1;

    }
}

unsigned char get_fifo(fifo_state_machine *buffer_struct, volatile unsigned char *buffer, unsigned char *char_item){
    //Check if buffer is empty!
    if(buffer_struct->inwaiting == 0){
        //Set debut overflow bit
        //buffer_struct->debug |= BIT0;
        //Return 0 to indicate failure (buffer full!)
        return 0; //Failure (nothing to get)
    }
    //Buffer ready to accept new item
    else{
        unsigned char i;
        for(i=0;i<buffer_struct->element_size;i++){
            //Copy value of the FIFO ring buffer tail into the get_byte pointer
            char_item[i]= buffer[buffer_struct->tail+i];
        }

        //Increment head location of ring buffer, modulus allows easy "wrapping" (circle)
        buffer_struct->tail = (buffer_struct->tail+buffer_struct->element_size)%buffer_struct->buffer_size;

        //Increment the inwaiting variable to account for the new item in the FIFO
        buffer_struct->inwaiting--; //Decrement the inwaiting count

        //Return 1 to indicate success
        //DEBUG - MAX INWAIT
        return 1; //Success

    }
}

unsigned char fifo_empty(fifo_state_machine *buffer_struct){
    if(buffer_struct->inwaiting==0){
        return 1;
    }
    else{
        return 0;
    }
}

void init_self_test_fifo(void){
    //Application FIFO
    selftest_state_machine.debug = 0;
    selftest_state_machine.element_size = 1;
    selftest_state_machine.head = 0;
    selftest_state_machine.inwaiting = 0;
    //rf_datalink_tx_fifo_state_machine.length = 0;
    selftest_state_machine.max_inwait = 0;
    selftest_state_machine.tail = 0;
    selftest_state_machine.buffer_size = 256;
}

void fifo_selftest(void){
    init_self_test_fifo();
    unsigned char testfifodata[5] = {0x00, 0x01, 0x02, 0x03, 0x04};
    unsigned char i;
    for(i=0; i<5; i++){
        put_fifo(&selftest_state_machine, &selftest_fifo_buffer, &testfifodata[i]);
        __no_operation();
    }


}
