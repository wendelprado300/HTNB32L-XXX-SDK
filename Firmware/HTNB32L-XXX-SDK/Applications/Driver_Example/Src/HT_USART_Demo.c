#include "HT_USART_Demo.h"
#include "htnb32lxxx_hal_usart.h"

extern USART_HandleTypeDef huart1;
extern USART_HandleTypeDef huart2;

static uint8_t rx_buffer_usart_1;
static uint8_t rx_buffer_usart_2;
// static uint8_t tx_buffer[50] = {0};
char cmdRxBuffer[255] = {0};
int cmdRxBufferIdx = 0;

volatile uint8_t rx_callback_usart_1 = 0;
volatile uint8_t tx_callback_usart_1 = 0;
volatile uint8_t rx_callback_usart_2 = 0;
volatile uint8_t tx_callback_usart_2 = 0;

extern uint8_t *usart_tx_buffer;
extern uint8_t *usart_rx_buffer;
extern uint32_t usart_tx_buffer_size;
extern uint32_t usart_rx_buffer_size;
uint32_t uart_cntrl = (ARM_USART_MODE_ASYNCHRONOUS | ARM_USART_DATA_BITS_8 | ARM_USART_PARITY_NONE | 
    ARM_USART_STOP_BITS_1 | ARM_USART_FLOW_CONTROL_NONE);


void HT_USART_Callback(uint32_t event) {
    rx_callback_usart_1 = 1;
}
void HT_USART_Callback_2(uint32_t event) {
    rx_callback_usart_2 = 1;
}

void HT_USART_App(void) {
    HAL_USART_Initialize(HT_USART_Callback_2, &huart2);
    HAL_USART_PowerControl(ARM_POWER_FULL, &huart2);
    HAL_USART_Control(uart_cntrl, 115200, &huart2);
    ht_printf("HTNB32L-XXX USART Example\n");
    
    while(1) {
        HAL_USART_ReceivePolling(&huart1, &rx_buffer_usart_1, 1);

        if (rx_buffer_usart_1 != 0 )
        {
            // ht_printf("%c",rx_buffer_usart_1);
            cmdRxBuffer[cmdRxBufferIdx] = rx_buffer_usart_1;
            cmdRxBufferIdx++;
            if (rx_buffer_usart_1 == '\r')
            {   
                HAL_USART_SendPolling(&huart2, (uint8_t *)cmdRxBuffer, strlen(cmdRxBuffer));
                 ht_printf("Received (user): %s\n", (char *)cmdRxBuffer);
                memset(&cmdRxBuffer,0,cmdRxBufferIdx);
                cmdRxBufferIdx = 0;
                do
                {
                    HAL_USART_ReceivePolling(&huart2, &rx_buffer_usart_2, 1); 
                    if (rx_buffer_usart_2 != 0 )
                    {
                    
                        if(rx_buffer_usart_2 != '\r')
                            rx_buffer_usart_2 = 0;
                    } 
                } while (rx_buffer_usart_2 != '\r');
            }
            HAL_USART_Control(ARM_USART_CONTROL_PURGE_COMM, 0, &huart1);
            HAL_USART_Control(ARM_USART_CONTROL_PURGE_COMM, 0, &huart2);
            rx_buffer_usart_1 = 0;
        }     
    }
    
}

/************************ HT Micron Semicondutores S.A *****END OF FILE****/