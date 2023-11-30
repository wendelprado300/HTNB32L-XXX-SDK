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

#include "HT_BSP_Custom.h"
#include "HT_hal_uart.h"

extern ARM_DRIVER_USART Driver_USART0;
extern ARM_DRIVER_USART Driver_USART1;

extern ARM_DRIVER_USART *UsartPrintHandle;

/* Function prototypes  ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn static void HT_GPR_SetUartClk(void)
 * \brief Setup UART clock.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_GPR_SetUartClk(void);

/*!******************************************************************
 * \fn static void HT_PMU_WakeupPadInit(void)
 * \brief Configures wakeup pin for this application.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_PMU_WakeupPadInit(void);

/* ---------------------------------------------------------------------------------------*/

static void HT_GPR_SetUartClk(void) {
    GPR_ClockDisable(GPR_UART0FuncClk);
    GPR_SetClockSrc(GPR_UART0FuncClk, GPR_UART0ClkSel_26M);
    GPR_ClockEnable(GPR_UART0FuncClk);
    GPR_SWReset(GPR_ResetUART0Func);
}

static void HT_PMU_WakeupPadInit(void) {
    const padWakeupSettings_t cfg = {
        false, true,              // group0 posedge, negedge -- (group0 = WKP_Pin0, WKP_Pin1)
        false, false,             // group1 posedge, negedge -- (group1 = WKP_Pin2, WKP_Pin3)
        false, false,             // group2 posedge, negedge -- (group2 = WKP_Pin4, WKP_Pin5)
    };

    slpManSetWakeupPad(cfg);
}

void HT_BSP_Init(void) {
    SystemCoreClockUpdate();

    CLOCK_DriverInit();

    PAD_DriverInit();

    GPR_Initialize();

    //interrupt config
    IC_PowupInit();

    BSP_CustomInit();
}

void HT_BSP_Deinit(void) {
    CLOCK_DriverInit();

	if (UsartPrintHandle != NULL) {
		UsartPrintHandle->PowerControl(ARM_POWER_OFF);
		UsartPrintHandle->Uninitialize();
		UsartPrintHandle = NULL;
	}

	if (UsartUnilogHandle != NULL) {
		UsartUnilogHandle->PowerControl(ARM_POWER_OFF);
		UsartUnilogHandle->Uninitialize();
		UsartUnilogHandle = NULL;
	}
}

void BSP_CustomInit(void) {
    uint32_t uart_cntrl = (ARM_USART_MODE_ASYNCHRONOUS | ARM_USART_DATA_BITS_8 | ARM_USART_PARITY_NONE | 
                                ARM_USART_STOP_BITS_1 | ARM_USART_FLOW_CONTROL_NONE);

    extern void mpu_init(void);
    mpu_init();

    HT_GPR_SetUartClk();

    HT_UART_InitPrint(HT_UART1, GPR_UART1ClkSel_26M, uart_cntrl, 115200);

    plat_config_raw_flash_t* rawFlashPlatConfig;

    BSP_LoadPlatConfigFromRawFlash();
    rawFlashPlatConfig = BSP_GetRawFlashPlatConfig();

    if(rawFlashPlatConfig && (rawFlashPlatConfig->logControl != 0 ))
    {
        SetUnilogUart(PORT_USART_0, rawFlashPlatConfig->uartBaudRate, false);
        uniLogInitStart(UART_0_FOR_UNILOG);
        HT_STRING(UNILOG_PLA_STRING, build_info, P_SIG, "%s", getBuildInfo());
    }

    ////////// wakeup pad 0 for button wakeup ///////////////
    HT_PMU_WakeupPadInit();
    NVIC_EnableIRQ(PadWakeup0_IRQn);

}

void Pad0_WakeupIntHandler(void) {
    if(slpManExtIntPreProcess(PadWakeup0_IRQn)==false)
        return;

    // add custom code here //
}

void Pad1_WakeupIntHandler(void) {
    if(slpManExtIntPreProcess(PadWakeup1_IRQn)==false)
        return;

    // add custom code here //
}

void Pad2_WakeupIntHandler(void) {
    if(slpManExtIntPreProcess(PadWakeup2_IRQn)==false)
        return;

    // add custom code here //
}

void Pad3_WakeupIntHandler(void) {
    if(slpManExtIntPreProcess(PadWakeup3_IRQn)==false)
        return;

    // add custom code here //
}

void Pad4_WakeupIntHandler(void) {
    if(slpManExtIntPreProcess(PadWakeup4_IRQn)==false)
        return;

    // add custom code here //
}

void Pad5_WakeupIntHandler(void) {
    if(slpManExtIntPreProcess(PadWakeup5_IRQn)==false)
        return;

    // add custom code here //
}

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
