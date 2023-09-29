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

static void HT_I2C_AppInit(void) {
    i2c.hi2c = &Driver_I2C0;
    i2c.i2c_id = HT_I2C0;
    i2c.cb_event = NULL;
    i2c.power_state = ARM_POWER_FULL;
    i2c.ctrl = I2C_BUS_SPEED_MASK | I2C_BUS_CLEAR_MASK;
    i2c.bus_speed = ARM_I2C_BUS_SPEED_FAST;
    i2c.clear_val = 1;

    HT_I2C_Init(&i2c);
}

void HT_I2C_App(void) {
    char str[RX_BUFFER_SIZE+25] = {'\0'};
    uint32_t len = TX_BUFFER_SIZE-1;

    HT_I2C_AppInit();

    print_uart("I2C Example Start!\n");

    while (1) {
        sprintf(str, "Transmitting: %s\n", (char *)tx_buffer);
        print_uart(str);
        HT_I2C_MasterTransmit(SLAVE_I2C_ADDRESS, tx_buffer, len);

        delay_us(50000);
        
        do {
            HT_I2C_MasterReceive(SLAVE_I2C_ADDRESS, rx_buffer, RX_BUFFER_SIZE-1);
        } while (rx_buffer[0] == '\0');
        
        sprintf(str, "I2C Received: %s\n", (char *)rx_buffer);
        print_uart(str);

        memset(str, '\0', sizeof(str));
        memset(rx_buffer, '\0', sizeof(rx_buffer));
        
        delay_us(1000000);
    }
    
}

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
