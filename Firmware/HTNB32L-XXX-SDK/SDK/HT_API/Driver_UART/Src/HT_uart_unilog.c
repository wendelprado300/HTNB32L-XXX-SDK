
#include "HT_uart_unilog.h"
#include "HT_Peripheral_Config.h"

extern ARM_DRIVER_USART Driver_USART0;
extern ARM_DRIVER_USART Driver_USART1;
ARM_DRIVER_USART *UsartUnilogHandle = NULL;

static uint8_t unilog_uart_recv_buf[UNILOG_RECV_BUFFER_SIZE];

/**
  \fn           void logToolCommandHandle(uint8_t *atcmd_buffer, uint32_t len)
  \brief        handle downlink command sent from unilog tool EPAT
                if need to handle more command in future, add command-handler table
  \param[in]    event         UART event, note used in this function
  \param[in]    cmd_buffer    command received from UART
  \param[in]    len           command length
  \returns      void
*/
void logToolCommandHandle(uint32_t event, uint8_t *cmd_buffer, uint32_t len) {
    (void)event;

    uint8_t * LogDbVserion=getDebugDVersion();

    if(ec_strnstr((const char *)cmd_buffer, "^logversion", len))
    {

        HT_STRING(UNILOG_PLA_INTERNAL_CMD, get_log_version, P_SIG, "LOGVERSION:%s",LogDbVserion);

    }
    else
    {

        HT_STRING(UNILOG_PLA_STRING, get_log_version_1, P_ERROR, "%s", (uint8_t*)"invalid command from EPAT");

    }

    return;

}

uint8_t* getUnilogRecvBuffer(void) {
    return unilog_uart_recv_buf;
}

void SetUnilogUart(usart_port_t port, uint32_t baudrate, bool startRecv) {
    hal_uart_config_t halUartConfig = {0};
    hal_uart_hardware_config_t hwConfig = {
                                            ARM_POWER_FULL,
                                            ARM_USART_MODE_ASYNCHRONOUS | ARM_USART_DATA_BITS_8 |
                                            ARM_USART_PARITY_NONE       | ARM_USART_STOP_BITS_1 |
                                            ARM_USART_FLOW_CONTROL_NONE,
                                            baudrate
                                           };

    if (port == PORT_USART_0) {
#if (RTE_UART0)
        UsartUnilogHandle = &CREATE_SYMBOL(Driver_USART, 0);
#endif

    } else if (port == PORT_USART_1) {
#if (RTE_UART1)
        UsartUnilogHandle = &CREATE_SYMBOL(Driver_USART, 1);
#endif
    }

    if (UsartUnilogHandle == NULL)
        return;

    halUartConfig.uartDriverHandler = UsartUnilogHandle;
    halUartConfig.recv_cb = (hal_uart_recv_cb_t)logToolCommandHandle;
    halUartConfig.recvBuffPtr = unilog_uart_recv_buf;
    halUartConfig.recvBuffSize = UNILOG_RECV_BUFFER_SIZE;

    if(startRecv) {
        HAL_UART_InitHandler(port, &halUartConfig, &hwConfig, HAL_UART_TASK_CREATE_FLAG_RECV);
    } else {
        HAL_UART_InitHandler(port, &halUartConfig, &hwConfig, HAL_UART_TASK_CREATE_FLAG_NONE);
    }
}