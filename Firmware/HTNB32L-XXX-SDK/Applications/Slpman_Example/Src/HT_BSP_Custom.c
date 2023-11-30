
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

#include "main.h"
#include "HT_BSP_Custom.h"

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

/*!******************************************************************
 * \fn static void UserTimerExpFunc(uint8_t id)
 * \brief Deep Sleep timer callback function.
 *
 * \param[in]  uint8_t id       timer ID.
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void UserTimerExpFunc(uint8_t id);

#if USE_AON_PIN == 1

/*!******************************************************************
 * \fn static void AONIOConfig(void)
 * \brief Always ON pin configuration. This function initializes an
 * AON pin that will keep its lo\gic level during a deep sleep state.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void AONIOConfig(void);

/*!******************************************************************
 * \fn static void AONIOInit(void)
 * \brief This function enables and initializes the HTNB32L-XXX AON pin.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void AONIOInit(void);

#endif 

/* ---------------------------------------------------------------------------------------*/

static void HT_GPR_SetUartClk(void) {
    GPR_ClockDisable(GPR_UART0FuncClk);
    GPR_SetClockSrc(GPR_UART0FuncClk, GPR_UART0ClkSel_26M);
    GPR_ClockEnable(GPR_UART0FuncClk);
    GPR_SWReset(GPR_ResetUART0Func);
}

#if LOW_POWER_AT_TEST

static slpManSlpState_t CheckUsrdefSlpStatus(void) {
    slpManSlpState_t status = SLP_HIB_STATE;
    if((slpManGetWakeupPinValue() & (0x1<<1)) == 0)         // pad1 value is low
        status = SLP_IDLE_STATE;
    else
        status = SLP_HIB_STATE;

    return status;
}

#endif

static void HT_PMU_WakeupPadInit(void) {
    const padWakeupSettings_t cfg = {
        false, true,              // group0 posedge, negedge -- (group0 = WKP_Pin0, WKP_Pin1)
        false, false,             // group1 posedge, negedge -- (group1 = WKP_Pin2, WKP_Pin3)
        false, false,             // group2 posedge, negedge -- (group2 = WKP_Pin4, WKP_Pin5)
    };

    slpManSetWakeupPad(cfg);
}

static void UserTimerExpFunc(uint8_t id) {
    HT_TRACE(UNILOG_PLA_APP, UserTimerExpFunc_1, P_SIG, 1, "User DeepSleep Timer Expired: TimerID = %u",id);
}

#if USE_AON_PIN == 1

static void AONIOConfig(void) {
    gpio_pin_config_t config;
    config.pinDirection = GPIO_DirectionOutput;
    config.misc.initOutput = 1;
    GPIO_PinConfig(AONIO0_INSTANCE_NUM, AONIO0_GPIO_PIN, &config);
}

static void AONIOInit(void) {
	slpManAONIOPowerOn();
	AONIOConfig();
}

#endif 

void BSP_CustomInit(void) {
    extern void mpu_init(void);
    mpu_init();

    HT_GPR_SetUartClk();

#if LOW_POWER_AT_TEST
    slpManRegisterUsrSlpDepthCb(CheckUsrdefSlpStatus);
#endif

    plat_config_raw_flash_t* rawFlashPlatConfig;

    BSP_LoadPlatConfigFromRawFlash();
    rawFlashPlatConfig = BSP_GetRawFlashPlatConfig();

    if(rawFlashPlatConfig && (rawFlashPlatConfig->logControl != 0 )) {
        SetUnilogUart(PORT_USART_0, rawFlashPlatConfig->uartBaudRate, true);
        uniLogInitStart(UART_0_FOR_UNILOG);
        HT_STRING(UNILOG_PLA_STRING, build_info, P_SIG, "%s", getBuildInfo());
    }

	slpManGetPMUSettings();				// get pmu setting from flash, we can overwrite after that

#if USE_AON_PIN == 1
	AONIOInit();
#endif

    ////////// wakeup pad 0 for button wakeup ///////////////
    HT_PMU_WakeupPadInit();
    NVIC_EnableIRQ(PadWakeup0_IRQn);

	slpManDeepSlpTimerRegisterExpCb(UserTimerExpFunc);			   // always register a user deepsleep timer callback
}

void Pad0_WakeupIntHandler(void) {
    if(slpManExtIntPreProcess(PadWakeup0_IRQn)==false)
        return;

    // add custom code below //
}

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
