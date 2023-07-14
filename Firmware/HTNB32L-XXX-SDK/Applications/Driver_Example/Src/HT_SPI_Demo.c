/**
 *
 * Copyright (c) 2023 HT Micron Semicondutors S.A.
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

#include "HT_SPI_Demo.h"
#include "bsp_spi.h"

static SPI_InitType spi_handle;

static __ALIGNED(4) uint8_t tx_buffer[] = {"HelloSpi1"};
static __ALIGNED(4) uint8_t rx_buffer[SPI_BUFFER_SIZE] = {0};

extern ARM_DRIVER_SPI Driver_SPI1;
extern SPI_RESOURCES SPI1_Resources;

static volatile uint8_t spi_callback = 0;

/*!******************************************************************
 * \fn static void HT_SPI_AppInit(void)
 * \brief SPI peripheral init.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_SPI_AppInit(void);

static void HT_SPI_AppInit(void) {
    spi_handle.cb_event = NULL;
    spi_handle.ctrl = (ARM_SPI_MODE_MASTER | ARM_SPI_CPOL0_CPHA0 | ARM_SPI_DATA_BITS(TRANSFER_DATA_WIDTH) |
                                  ARM_SPI_MSB_LSB | ARM_SPI_SS_MASTER_UNUSED);


    spi_handle.power_state = ARM_POWER_FULL;
    spi_handle.spi_id = HT_SPI1;
    spi_handle.hspi = &Driver_SPI1;

    HT_SPI_Init(&spi_handle);
}

void HT_SPI_App(void) {
    char tx_str[SPI_BUFFER_SIZE+25] = {'\0'};
    char rx_str[SPI_BUFFER_SIZE+25] = {'\0'};

    HT_SPI_AppInit();

    print_uart("HTNB32L-XXX SPI Example Start!\n");
    
    //Wait for slave device
    delay_us(2000000);

    sprintf(tx_str, "\nTransmitting: %s\n", (char *)tx_buffer);

    while (1) {

        print_uart(tx_str);
        HT_SPI_TransmitReceive(tx_buffer, rx_buffer, SPI_BUFFER_SIZE-1);

        sprintf(rx_str, "Received: %s\n", (char *)rx_buffer);
        print_uart(rx_str);

        delay_us(2000000);
        
    }
    
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
