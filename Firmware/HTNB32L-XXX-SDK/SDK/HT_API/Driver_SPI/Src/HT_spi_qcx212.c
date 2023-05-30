#include "HT_spi_qcx212.h"
#include "cmsis_gcc.h"

static ARM_DRIVER_SPI *spi_handle;

void HT_SPI_TransmitReceive(uint8_t *tx_buffer, uint8_t *rx_buffer, uint32_t len){
    spi_handle->Transfer(tx_buffer, rx_buffer, len);
}

void HT_SPI_Transmit(uint8_t *tx_buffer, uint32_t len) {
    spi_handle->Send(tx_buffer, len);
}

void HT_SPI_Receive(uint8_t *rx_buffer, uint32_t len) {
    spi_handle->Receive(rx_buffer, len);
}

void HT_SPI_Init(SPI_InitType *spi) {

    spi_handle = spi->hspi;

    spi_handle->Initialize(spi->cb_event);
    spi_handle->PowerControl(spi->power_state);

    spi_handle->Control(spi->ctrl, 200000U);
}

__attribute__((weak)) void HT_SPI_RxCallback(void) {
    __NOP();
}
