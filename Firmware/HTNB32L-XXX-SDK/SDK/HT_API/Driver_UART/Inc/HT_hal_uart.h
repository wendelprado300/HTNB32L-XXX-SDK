#ifndef __HT_HAL_UART_H__
#define __HT_HAL_UART_H__

#include "hal_uart.h"
#include "clock_qcx212.h"
#include "main.h"

typedef enum {
  HT_UART0 = 0,
  HT_UART1,
  HT_UART2
} HT_UART_Id;

/**
  \brief Initilize UART peripheral
  \param[in] uartIndex          uart index
  \param[in] config             pointer to configuration for initialization
  \param[in] hwConfig           pointer to uart hardware configuration for initialization
  \param[in] taskCreateFlag     flag to control whether to create send or receive task or not
  \return    error code
 */
int32_t HT_UART_InitHandler(uint8_t uartIndex, hal_uart_config_t *config, hal_uart_hardware_config_t *hwConfig, hal_uart_task_create_flag_t taskCreateFlag);

/**
  \brief Initilize HAL UART handler
  \param[in] uartIndex     uart index
  \param[in] newHwConfig   pointer to new uart hardware configuration
  \param[in] startRecv     flag for indicating whether start to receive after successful call or not
  \return    error code
 */
int32_t HT_UART_ResetUartSetting(uint8_t uartIndex, hal_uart_hardware_config_t *newHwConfig, bool startRecv);

/**
  \brief De-initilize HAL UART handler
  \param[in] uartIndex  uart index
  \return    void
  */
void HT_UART_DeInitHandler(uint8_t uartIndex);

/**
  \brief    Send msg to uart send task
  \param[in] type         message type, \ref hal_uart_send_msg_type_t
  \param[in] uartIdx      uart index indicating which UART port the message will be sent to
  \param[in] msgPtr       pointer to message data
  \param[in] msgLen       lengh of message data in unit of byte
  \return    void
 */
void HT_UART_SendMsg(uint16_t type, uint8_t uartIdx, uint8_t *msgPtr, uint16_t msgLen);

/**
  \brief    Send string to uart send task for print
  \param[in] uartIdx      uart index indicating which UART port the message will be sent to
  \param[in] strPtr       pointer to string body
  \param[in] strLen       string length
  \return    void
 */
void HT_UART_SendStr(uint8_t uartIdx, uint8_t *strPtr, uint16_t strLen);

/**
  \brief    Bypass SEND task and send string to uart directly
  \param[in] uartIdx      uart index indicating which UART port the message will be sent to
  \param[in] strPtr       pointer to string body
  \param[in] strLen       string length
  \return    void
  \note     Used only in specific cases, such as sleep entering procedure where we should not switch out from IDLE task
 */
void HT_UART_DirectSendStr(uint8_t uartIdx, uint8_t *strPtr, uint16_t strLen);

/**
  \brief    HAL UART Recv flow control
  \param[in] on           recv control flag, set true to disable recv and false to enable recv
  \return    void
 */
void HT_UART_RecvFlowControl(bool on);

/**
  \brief    Initialize UART for print purposes. 
  \param[in] uart         ID of the choosen UART (0,1 or 2)
  \param[in] uartClkSel   Clock source for the UART peripheral.
  \param[in] control      Control word for ARM_USART_Control. Please check this function call in order to understand how this word
                          must be mounted.
  \param[in] baudrate     UART Baudrate.
  \return    void
 */
void HT_UART_InitPrint(HT_UART_Id uart, clock_select_t uartClkSel, uint32_t control, uint32_t baudrate);

/**
  \brief    Set UART0 clock, used for log purposes.
  \param[in] uartClkSel   Clock source for the UART peripheral.
  \return    void
 */
void HT_UART_SetUARTLogClk(clock_select_t uartClkSel);

/**
  \brief    Configures an UART for log purposes.
  \param[in] uart   UART index.
  \return    void
 */
void HT_UART_ConfigUartUnilog(HT_UART_Id uart);

#if HT_DRIVER_EXAMPLE == 1
void print_uart(char *str);
#endif 

#endif /* __HT_HAL_UART_H__ */
