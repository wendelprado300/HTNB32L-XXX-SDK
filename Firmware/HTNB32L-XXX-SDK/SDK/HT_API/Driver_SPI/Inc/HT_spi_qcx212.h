#ifndef __HT_SPI_QCX212_H__
#define __HT_SPI_QCX212_H__

#include "stdint.h"
#include "Driver_Common.h"
#include "Driver_SPI.h"

typedef enum {
    HT_SPI0 = 0,
    HT_SPI1
} HT_SPI_Type;

typedef struct  {
    ARM_DRIVER_SPI *hspi;
    HT_SPI_Type spi_id;
    ARM_POWER_STATE power_state;
    ARM_SPI_SignalEvent_t cb_event;
    uint32_t ctrl;
} SPI_InitType;

void HT_SPI_Init(SPI_InitType *spi);
void HT_SPI_TransmitReceive(uint8_t *tx_buffer, uint8_t *rx_buffer, uint32_t len);
void HT_SPI_Transmit(uint8_t *tx_buffer, uint32_t len);
void HT_SPI_Receive(uint8_t *rx_buffer, uint32_t len);

void HT_SPI_RxCallback(void);

#endif /*__HT_SPI_QCX212_H__*/
