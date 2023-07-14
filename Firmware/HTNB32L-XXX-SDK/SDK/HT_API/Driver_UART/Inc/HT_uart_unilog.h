#ifndef __HT_UART_UNILOG_H__
#define __HT_UART_UNILOG_H__

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "clock_qcx212.h"
#include "bsp.h"
#include "system_qcx212.h"
#include "debug_log.h"
#include "hal_uart.h"
#include "mem_map.h"

#define UNILOG_RECV_BUFFER_SIZE       96

void logToolCommandHandle(uint32_t event, uint8_t *cmd_buffer, uint32_t len);
uint8_t * getUnilogRecvBuffer(void);
void SetUnilogUart(usart_port_t port, uint32_t baudrate, bool startRecv);

#endif /*__HT_UART_UNILOG_H__*/
