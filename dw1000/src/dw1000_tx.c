/*! ----------------------------------------------------------------------------
 *  @file    main.c
 *  @brief   Simple TX example code
 *
 * @attention
 *
 * Copyright 2015 (c) Decawave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author Decawave
 */
 
 /*
 * Modified by:
 * Anh Luong <luong@eng.utah.edu>
 * Chenxi Wang <chenxiwa@andrew.cmu.edu>
 */

#include <stdio.h>
#include <stdlib.h>  // malloc, free
#include <unistd.h>
#include <stdint.h>
#include <string.h> // memset
#include <time.h>

#include "deca_device_api.h"
#include "deca_regs.h"
#include "platform.h"

#define APP_NAME "HEADCOUNT TX v2.0"

/* Default communication configuration. We use here EVK1000's default mode (mode 3). */
static dwt_config_t config = {
    2,               /* Channel number. */
    DWT_PRF_64M,     /* Pulse repetition frequency. */
    DWT_PLEN_1024,   /* Preamble length. Used in TX only. */
    DWT_PAC32,       /* Preamble acquisition chunk size. Used in RX only. */
    9,               /* TX preamble code. Used in TX only. */
    9,               /* RX preamble code. Used in RX only. */
    1,               /* 0 to use standard SFD, 1 to use non-standard SFD. */
    DWT_BR_110K,     /* Data rate. */
    DWT_PHRMODE_STD, /* PHY header mode. */
    (1025 + 64 - 32) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};

/* Index to access to a certain frame in the tx_msg array. */
#define FLAG_IDX   1   // flag index

/* Number of messages sent per one call. */
#define BATCH_NUM 100

/* Inter-frame delay period, in milliseconds. */
#define TX_SLOT_MS 50

typedef unsigned long long uint64;
typedef signed long long int64;

static void setup_dw1000(void) {
    
    /* Reset and initialise DW1000.
     * For initialisation, DW1000 clocks must be temporarily set to crystal speed. After initialisation SPI rate can be increased for optimum performance.
     */
    reset_DW1000(); /* Target specific drive of RSTn line into DW1000 low for a period. */
    spi_set_rate_low();
    
    if (dwt_initialise(DWT_LOADUCODE) == DWT_ERROR)
    {
        printf("%s\n", "INIT FAILED");
        exit(1);
    }
    spi_set_rate_high();
    
    /* Configure DW1000. See NOTE 7 below. */
    dwt_configure(&config);
    
    printf("%s\n", APP_NAME);
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn get_system_timestamp_u64()
 *
 * @brief Get the system in a 64-bit variable.
 *
 * @param  none
 *
 * @return  64-bit value of the system time.
 */
static uint64 get_system_timestamp_u64(void) {
    uint8 ts_tab[5];
    uint64 ts = 0;
    int i;
    dwt_readsystime(ts_tab);
    for (i = 4; i >= 0; i--)
    {
        ts <<= 8;
        ts |= ts_tab[i];
    }
    return ts;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn initiator()
 *
 * @brief Send the MSG for given time slot and batch number.
 *
 * @param  none
 *
 * @return  none
 */
static void initiator(void){
    /******** Variable Define *********/
    uint8 tx_msg[] = {0xab, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    /*The frame sent in this example is adjusted from an 802.15.4e standard blink. It is a 12-byte frame composed of the following fields:
     *     - byte 0: frame type (0xC5 for a blink).
     *     - byte 1: not used here
     *     - byte 2 -> 9: current time
     *     - byte 10/11: frame check-sum, automatically set by DW1000.
     size = 1+1+8+2 = 12
     */
    char flag = 0;
    /* Frequency Control */
    time_t start;
    double duration;
    
    /******** Batch MSG sending loop *********/
    for(uint64 seq=1; seq<=BATCH_NUM; seq++){
        start = clock();
        t1 = std::chrono::steady_clock::now();
        memcpy((void *) &tx_msg[FLAG_IDX], (void *) &seq, sizeof(uint64));
        flag = !flag;
        /* Write frame data to DW1000 and prepare transmission. See NOTE 4 below.*/
        dwt_writetxdata(sizeof(tx_msg), tx_msg, 0); /* Zero offset in TX buffer. */
        dwt_writetxfctrl(sizeof(tx_msg), 0, 0); /* Zero offset in TX buffer, no ranging. */
        
        /* Start transmission. */
        dwt_starttx(DWT_START_TX_IMMEDIATE);
        
        /* Poll DW1000 until TX frame sent event set. See NOTE 5 below.
         * STATUS register is 5 bytes long but, as the event we are looking at is in the first byte of the register, we can use this simplest API
         * function to access it.*/
        while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS))
        { };
        
        /* Clear TX frame sent event. */
        dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);
        printf("%llu MSG SENT!\r\n", seq);
        
        /* Frequency Control */
        do {
            duration = (double)1000*(clock() - start)/CLOCKS_PER_SEC;
        } while (duration<TX_SLOT_MS);
        printf("%f\r\n", duration);
    }
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn main()
 *
 * @brief Application entry point.
 *
 * @param  none
 *
 * @return none
 */
int main(void)
{
    /** Initialization **/
    
    /* Start with board specific hardware init. */
	hardware_init();
    setup_dw1000();
    
    
    /** MSG Sending Loop **/
    initiator();
}

/*****************************************************************************************************************************************************
 * NOTES:
 *
 * 1. The device ID is a hard coded constant in the blink to keep the example simple but for a real product every device should have a unique ID.
 *    For development purposes it is possible to generate a DW1000 unique ID by combining the Lot ID & Part Number values programmed into the
 *    DW1000 during its manufacture. However there is no guarantee this will not conflict with someone else’s implementation. We recommended that
 *    customers buy a block of addresses from the IEEE Registration Authority for their production items. See "EUI" in the DW1000 User Manual.
 * 2. In this example, LDE microcode is not loaded upon calling dwt_initialise(). This will prevent the IC from generating an RX timestamp. If
 *    time-stamping is required, DWT_LOADUCODE parameter should be used. See two-way ranging examples (e.g. examples 5a/5b).
 * 3. In a real application, for optimum performance within regulatory limits, it may be necessary to set TX pulse bandwidth and TX power, (using
 *    the dwt_configuretxrf API call) to per device calibrated values saved in the target system or the DW1000 OTP memory.
 * 4. dwt_writetxdata() takes the full size of tx_msg as a parameter but only copies (size - 2) bytes as the check-sum at the end of the frame is
 *    automatically appended by the DW1000. This means that our tx_msg could be two bytes shorter without losing any data (but the sizeof would not
 *    work anymore then as we would still have to indicate the full length of the frame to dwt_writetxdata()).
 * 5. We use polled mode of operation here to keep the example as simple as possible but the TXFRS status event can be used to generate an interrupt.
 *    Please refer to DW1000 User Manual for more details on "interrupts".
 * 6. The user is referred to DecaRanging ARM application (distributed with EVK1000 product) for additional practical example of usage, and to the
 *    DW1000 API Guide for more details on the DW1000 driver functions.
 ****************************************************************************************************************************************************/

