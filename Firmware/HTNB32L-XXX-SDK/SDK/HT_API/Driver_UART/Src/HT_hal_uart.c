#include "HT_hal_uart.h"
#include "bsp.h"
#include "HT_bsp.h"
#include "HT_clock_qcx212.h"
#include "plat_config.h"
#include "unilog_qcx212.h"
#include "debug_trace.h"
#include "HT_plat_config.h"
#include "HT_Peripheral_Config.h"

#if (RTE_UART0)
extern ARM_DRIVER_USART Driver_USART0;
#endif

#if (RTE_UART1)
extern ARM_DRIVER_USART Driver_USART1;
#endif

#if (RTE_UART2)
extern ARM_DRIVER_USART Driver_USART2;
#endif

ARM_DRIVER_USART *UsartPrintHandle = NULL;

void HT_UART_InitPrint(HT_UART_Id uart, clock_select_t uartClkSel, uint32_t control, uint32_t baudrate) {

    switch (uart) {
    case HT_UART0:
     #if (RTE_UART0)
        UsartPrintHandle = &CREATE_SYMBOL(Driver_USART, 0);
        HT_GPR_ClockDisable(GPR_UART0FuncClk);        
        HT_GPR_SetClockSrc(GPR_UART0FuncClk, uartClkSel);
        HT_GPR_ClockEnable(GPR_UART0FuncClk);
    #endif         
        break;
    case HT_UART1:
    #if (RTE_UART1)
        UsartPrintHandle = &CREATE_SYMBOL(Driver_USART, 1);
        HT_GPR_ClockDisable(GPR_UART1FuncClk);        
        HT_GPR_SetClockSrc(GPR_UART1FuncClk, uartClkSel);
        HT_GPR_ClockEnable(GPR_UART1FuncClk);
    #endif
        break;
    
    case HT_UART2:
     #if (RTE_UART2)
        UsartPrintHandle = &CREATE_SYMBOL(Driver_USART, 2);
        HT_GPR_ClockDisable(GPR_UART2FuncClk);        
        HT_GPR_SetClockSrc(GPR_UART2FuncClk, GPR_UART2ClkSel_26M);
        HT_GPR_ClockEnable(GPR_UART2FuncClk);
    #endif
        break;
    }

    if(UsartPrintHandle == NULL)
        return;

    UsartPrintHandle->Initialize(NULL);
    UsartPrintHandle->PowerControl(ARM_POWER_FULL);
    UsartPrintHandle->Control(control, baudrate);
}

void HT_UART_SetUARTLogClk(clock_select_t uartClkSel) {
    HT_GPR_ClockDisable(GPR_UART0FuncClk);
    HT_GPR_SetClockSrc(GPR_UART0FuncClk, uartClkSel);
    HT_GPR_ClockEnable(GPR_UART0FuncClk);
    HT_GPR_SWReset(GPR_ResetUART0Func);
}

void HT_UART_ConfigUartUnilog(HT_UART_Id uart) {
    plat_config_raw_flash_t *rawFlashPlatConfig;

    HT_BSP_LoadPlatConfigFromRawFlash();
    rawFlashPlatConfig = (plat_config_raw_flash_t *)HT_BSP_GetRawFlashPlatConfig();

    if(rawFlashPlatConfig && (rawFlashPlatConfig->logControl != 0 )) {
        HT_SetUnilogUart(PORT_USART_0, rawFlashPlatConfig->uartBaudRate, true);
        uniLogInitStart(UART_0_FOR_UNILOG);
    }
}

/*
 *  retarget for _write implementation
 *  Parameter:      ch: character will be out
 */
int io_putchar(int ch)
{
    if (UsartPrintHandle != NULL)
        UsartPrintHandle->Send((uint8_t*)&ch, 1);
    return 0;
}

/*
 *  retarget for _read implementation
 *  Parameter:      ch: character will be read
 */
int io_getchar()
{
    uint8_t ch = 0;
    if (UsartPrintHandle != NULL)
        UsartPrintHandle->Receive(&ch, 1);
    return (ch);
}

int fgetc(FILE *f)
{
    uint8_t ch = 0;
    if (UsartPrintHandle != NULL)
        UsartPrintHandle->Receive(&ch, 1);
    return (ch);
}


__attribute__((weak,noreturn))
void __aeabi_assert (const char *expr, const char *file, int line) {
  printf("Assert, expr:%s, file: %s, line: %d\r\n", expr, file, line);
  while(1);
}

int _write(int file, char *ptr, int len) {
	//extern int io_putchar(int ch);
    int DataIdx;

    for (DataIdx = 0; DataIdx < len; DataIdx++) {
		io_putchar(*ptr++);
    }
    return len;
}
