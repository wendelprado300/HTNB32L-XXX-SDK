/**
 *
 * Copyright (c) 2023 HT Micron Semicondutores S.A.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "HT_I2C_Demo.h"
#include <stdio.h>
#include <stdlib.h>

static I2C_InitType i2c = {0};
static uint8_t tx_buffer[TX_BUFFER_SIZE] = {"HelloI2c0"};
static uint8_t rx_buffer[RX_BUFFER_SIZE] = {0};

extern ARM_DRIVER_I2C Driver_I2C0;

static volatile uint8_t i2c_tx_callback = 0;
static volatile uint8_t i2c_rx_callback = 0;

/* Function prototypes  ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn static void HT_I2C_AppInit(void)
 * \brief Initialize I2C peripheral for Driver Example.
 *
 * \param[in]  none.
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_I2C_MasterReceiveCallback(uint32_t event);

/*!******************************************************************
 * \fn static void HT_I2C_AppInit(void)
 * \brief Initialize I2C peripheral for Driver Example.
 *
 * \param[in]  none.
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_I2C_AppInit(void);

/* ---------------------------------------------------------------------------------------*/

static void HT_I2C_MasterReceiveCallback(uint32_t event) {

    if(event & ARM_I2C_EVENT_TX_DONE) {
        i2c_tx_callback = 1;
    } else if(event & ARM_I2C_EVENT_RX_DONE) {
        i2c_rx_callback = 1;
    }

}

static void HT_I2C_AppInit(void) {
    i2c.hi2c = &Driver_I2C0;
    i2c.i2c_id = HT_I2C0;
    i2c.cb_event = HT_I2C_MasterReceiveCallback;
    i2c.power_state = ARM_POWER_FULL;
    i2c.ctrl = I2C_BUS_SPEED_MASK | I2C_BUS_CLEAR_MASK;
    i2c.bus_speed = ARM_I2C_BUS_SPEED_STANDARD;
    i2c.clear_val = 1;

    HT_I2C_Init(&i2c);
}

void HT_I2C_App(void) {

    // Initializes the I2C peripheral and its respective IRQ.
    HT_I2C_AppInit();

    print_uart("I2C Example Start!\n");

    // Waits a few of seconds for synchronization purposes
    delay_us(10000000);

    while (1) {
        print_uart("Transmitting...\n");

        // Transmits a buffer through I2C to a specific slave device
        HT_I2C_MasterTransmit(SLAVE_I2C_ADDRESS, tx_buffer, TX_BUFFER_SIZE-1);

        // Waits for the end of the transmission
        while(!i2c_tx_callback);
        i2c_tx_callback = 0;

        print_uart("Transmitted!\n");

        delay_us(3000000);

        // Receives a buffer through I2C from a specific slave device        
        HT_I2C_MasterReceive(SLAVE_I2C_ADDRESS, rx_buffer, RX_BUFFER_SIZE-1);

        // Waits for the RX callback generated after the amount of the expected data is received
        while(!i2c_rx_callback);
        i2c_rx_callback = 0;
        
        print_uart("I2C Received: ");
        print_uart((char *)rx_buffer);
        print_uart("\n");

        // Clear I2C rx buffer
        memset(rx_buffer, 0, sizeof(rx_buffer));        
        delay_us(10000000);
    }
    
}

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
