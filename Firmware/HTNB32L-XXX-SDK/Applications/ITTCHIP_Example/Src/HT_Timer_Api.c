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

#include "HT_Timer_Api.h"

/*!******************************************************************
 * \fn static void HT_Timer_Init(void)
 * \brief Initialize Timer
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_Timer_Init(int seconds);

/** \brief timer interupt flag */
volatile int timerInterrupt = 1;

/**
  \fn          void HT_Timer0_ISR(void)
  \brief       timer0 interrupt service routine
  \return
*/
static void HT_Timer0_ISR(void) {
    timerInterrupt=1;
}

/**
  \fn          static void HT_Timer_Init(int seconds)
  \brief       Configures Timer function
  \return
*/
static void HT_Timer_Init(int seconds) {

    timer_config_t timerConfig;

    // Config TIMER clock, source from 32.768KHz and divide by 1
    CLOCK_SetClockSrc(GPR_TIMER0FuncClk, GPR_TIMER0ClkSel_32K);
    CLOCK_SetClockDiv(GPR_TIMER0FuncClk, 1);

    TIMER_DriverInit();

    // Config timer period as 1s, match0 value is 32768 = 0x8000
    TIMER_GetDefaultConfig(&timerConfig);
    timerConfig.reloadOption = TIMER_ReloadOnMatch0;
    timerConfig.match0 = 0x8000*seconds;

    TIMER_Init(0, &timerConfig);

    // Note interrupt flag won't assert in TIMER_InterruptPulse mode
    TIMER_InterruptConfig(0, TIMER_Match0Select, TIMER_InterruptPulse);
    TIMER_InterruptConfig(0, TIMER_Match1Select, TIMER_InterruptDisabled);
    TIMER_InterruptConfig(0, TIMER_Match2Select, TIMER_InterruptDisabled);

    // Enable IRQ
    XIC_SetVector(PXIC_Timer0_IRQn, HT_Timer0_ISR);
    XIC_EnableIRQ(PXIC_Timer0_IRQn);
}

/**
  \fn          void HT_Timer_App(void)
  \brief       Initializes Timer
  \return
*/
void HT_Timer_App(void) {
    printf("Timer Start!\n");

    HT_Timer_Init(periodic_time_seconds);
    TIMER_Start(0);  
}

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
