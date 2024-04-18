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
#include "htnb32lxxx_hal_i2c.h"

static uint8_t tx_buffer[TX_BUFFER_SIZE] = {"HelloI2c0"};
static uint8_t rx_buffer[RX_BUFFER_SIZE] = {0};

extern ARM_DRIVER_I2C Driver_I2C0;

static volatile uint8_t i2c_tx_callback = 0;
static volatile uint8_t i2c_rx_callback = 0;

extern I2C_HandleTypeDef hi2c0;

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
    HAL_I2C_InitClock(HT_I2C0);

    HAL_I2C_Initialize(HT_I2C_MasterReceiveCallback, &hi2c0);
    HAL_I2C_PowerControl(ARM_POWER_FULL, &hi2c0);
    
    HAL_I2C_Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD, &hi2c0);
    HAL_I2C_Control(ARM_I2C_BUS_CLEAR, 1, &hi2c0);
}

void HT_I2C_App(void) {

    // Initializes the I2C peripheral and its respective IRQ.
    HT_I2C_AppInit();

    ht_printf("iMCP HTNB32L-XXX - I2C Example Start!\n");

    ht_printf("Transmitting...\n");
    // Transmits a buffer through I2C to a specific slave device
    HAL_I2C_MasterTransmit_IT(&hi2c0, SLAVE_I2C_ADDRESS, tx_buffer, TX_BUFFER_SIZE-1);

    while (1) {
        
        
        // Waits for the end of the transmission
        if(i2c_tx_callback) {
            i2c_tx_callback = 0;
            ht_printf("Transmitted!\n");

            ht_printf("Receiving...\n");
            
            // Clear I2C rx buffer
            memset(rx_buffer, 0, sizeof(rx_buffer));

            delay_us(1000000);
            // Receives a buffer through I2C from a specific slave device        
            HAL_I2C_MasterReceive_IT(&hi2c0, SLAVE_I2C_ADDRESS, rx_buffer, RX_BUFFER_SIZE-1);
        }
        
        // Waits for the RX callback generated after the amount of the expected data is received
        if(i2c_rx_callback) {    
            i2c_rx_callback = 0;

            ht_printf("I2C Received: %s\n", (char *)rx_buffer);

            ht_printf("Transmitting...\n");
            delay_us(5000000);

            // Transmits a buffer through I2C to a specific slave device
            HAL_I2C_MasterTransmit_IT(&hi2c0, SLAVE_I2C_ADDRESS, tx_buffer, TX_BUFFER_SIZE-1);
        }

    }
    
}

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
