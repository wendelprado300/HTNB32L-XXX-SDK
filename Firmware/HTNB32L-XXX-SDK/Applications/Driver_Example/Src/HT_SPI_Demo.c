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

#include "HT_SPI_Demo.h"
#include "htnb32lxxx_hal_spi.h"

static __ALIGNED(4) uint8_t tx_buffer[] = {"HelloSpi1"};
static __ALIGNED(4) uint8_t rx_buffer[SPI_BUFFER_SIZE] = {0};

extern SPI_HandleTypeDef hspi1;
volatile uint8_t spi_irq = 0;

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

static void HT_SPI_IRQCallback(uint32_t event) {
    if(event & ARM_SPI_EVENT_TX_RX_COMPLETE)
        spi_irq = 1;
}

static void HT_SPI_AppInit(void) {
    uint32_t ctrl = (ARM_SPI_MODE_MASTER | ARM_SPI_CPOL0_CPHA0 | ARM_SPI_DATA_BITS(TRANSFER_DATA_WIDTH) | ARM_SPI_MSB_LSB | ARM_SPI_SS_MASTER_UNUSED);

    HAL_SPI_Initialize(HT_SPI_IRQCallback, &hspi1);
    HAL_SPI_PowerControl(ARM_POWER_FULL, &hspi1);
    HAL_SPI_Control(ctrl, 100000U, &hspi1);
}

void HT_SPI_App(void) {

    HT_SPI_AppInit();
    HAL_SPI_CleanRxFifo(&hspi1);

    print_uart("HTNB32L-XXX SPI Example Start!\n");
    
    //Wait for slave device
    delay_us(10000000);

    hspi1.info->xfer.num = (SPI_BUFFER_SIZE-1);
    hspi1.info->transfer_type = SPI_TRANSMIT_RECEIVE;

    while (1) {

        print_uart("Transmitting: ");
        print_uart((char *)tx_buffer);
        print_uart("\n");

        hspi1.info->xfer.tx_cnt   = 0;
        hspi1.info->xfer.rx_cnt   = 0;

        HAL_SPI_TransmitReceive_IT(&hspi1, tx_buffer, rx_buffer, SPI_BUFFER_SIZE-1);
        HAL_SPI_EnableIRQ(&hspi1);

        while(!spi_irq);
        spi_irq = 0;

        print_uart("Received: ");
        print_uart((char *)rx_buffer);
        print_uart("\n");
        
        memset(rx_buffer, 0, sizeof(rx_buffer));
        HAL_SPI_CleanRxFifo(&hspi1);

        delay_us(1000000);
        
    }
    
}

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
