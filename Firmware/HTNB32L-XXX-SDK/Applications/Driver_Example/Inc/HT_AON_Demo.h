/*

  _    _ _______   __  __ _____ _____ _____   ____  _   _
 | |  | |__   __| |  \/  |_   _/ ____|  __ \ / __ \| \ | |
 | |__| |  | |    | \  / | | || |    | |__) | |  | |  \| |
 |  __  |  | |    | |\/| | | || |    |  _  /| |  | | . ` |
 | |  | |  | |    | |  | |_| || |____| | \ \| |__| | |\  |
 |_|  |_|  |_|    |_|  |_|_____\_____|_|  \_\\____/|_| \_|
 =================== Advanced R&D ========================

 Copyright (c) 2023 HT Micron Semicondutores S.A.
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 http://www.apache.org/licenses/LICENSE-2.0
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

*/

/*!
 * \file HT_AON_Demo.h
 * \brief AON demonstration app. 
  * \author HT Micron Advanced R&D,
 *         Hêndrick Bataglin Gonçalves
 * 
 * \link https://github.com/htmicron
 * \version 0.1
 * \date October 03, 2023
 */

#ifndef __HT_AON_API_H__
#define __HT_AON_API_H__

#include "main.h"
#include "slpman_qcx212.h"
#include "pad_qcx212.h"
#include "HT_gpio_qcx212.h"
#include "ic_qcx212.h"
#include "HT_ic_qcx212.h"
#include "ecpm_qcx212.h"

/* Defines  ------------------------------------------------------------------*/

/*
----------------------------------------------------  Table 1. GPIO Table. -------------------------------------------------------------
___________________________________________________________________________________________________________________________________________________________________________________________
| Pad ID |       GPIO Number    |  Pin Number  | Instance | Pull (default:options) |       AF0      |      AF1      |    AF2      |      AF3      |  AF4 |   AF5  |   AF6  |      AF7      |
|--------|----------------------|--------------|----------|------------------------|----------------|---------------|-------------|---------------|------|--------|--------|---------------|
|   12   |        GPIO1         |      1       |    0     |      B-PU:nppd         |  GPIO1         |      –	      |  UART2_TXD  |      –        |   –  |    –   |    –   |      –        | 
|   13   |        GPIO2         |      2       |    0     |      B-PU:nppd         |  GPIO2/Timer0  |  UART0_RTSn   |  UART2_RXD  |    SPI1_SSn0  |   –  |  PWM0  |    –   |      –        |
|   14   |        GPIO3         |      3       |    0     |      B-PU:nppd         |  GPIO3         |  UART0_CTSn   |  UART2_TXD  |    SPI1_MOSI  |   –  |  PWM1  |    –   |      –        |
|   15   |        GPIO4         |      4       |    0     |      B-PU:nppd         |  GPIO4         |  UART0_RXD    |  I2C1_SDA   |    SPI1_MISO  |   –  |  PWM2  |    –   |      –        |
|   16   |        GPIO5         |      5       |    0     |      B-PU:nppd         |  GPIO5         |  UART0_TXD    |  I2C1_SCL   |    SPI1_SCLK  |   –  |  PWM3  |    –   |      –        |
|   17   |        GPIO6         |      6       |    0     |      B-PU:nppd         |  GPIO6/Timer1  |  SPI0_SSn0    |  I2C0_SDA   |    UART1_RTSn |   –  |  PWM4  |    –   |      –        |
|   18   |        GPIO7         |      7       |    0     |      B-PU:nppd         |  GPIO7         |  SPI0_MOSI    |  I2C0_SCL   |    UART1_CTSn |   –  |  PWM5  |    –   |      –        |
|   19   |        GPIO13        |      13      |    0     |      B-PU:nppd         |  GPIO13        |  SPI0_MISO    |  I2C1_SDA   |    UART1_RXD  |   –  |  PWM0  |    –   |      –        |
|   20   |        GPIO12        |      12      |    0     |      B-PU:nppd         |  GPIO12        |  SPI0_SCLK    |  I2C1_SCL   |    UART1_TXD  |   –  |  PWM1  |    –   |      –        |
|   25   |        GPIO10        |      10      |    0     |      B-PU:nppd         |  GPIO10/Timer3 |  I2C0_SCL     |      –      |    SPI1_SSn1  |   –  |  PWM0  |    –   |      –        |
|   9    |        SWCLK         |      2       |    1     |      B-PU:nppd         |  SWCLK         |      –        |  UART2_RXD  |    UART1_RTSn |   –  |  PWM4  |    –   |    GPIO18     |
|   10   |        SWDIO         |      3       |    1     |      B-PU:nppd         |  SWDIO         |      –        |  UART2_TXD  |    UART1_CTSn |   –  |  PWM5  |    –   |    GPIO19     |
|   31   |   AON_GPIO0 (GPIO20)	|      4       |    1     |      DIO-PD:nppu       |  GPIO20/Timer5 |      –        |      –      |      –        |   –  |    –   |    –   |      –        |
____________________________________________________________________________________________________________________________________________________________________________________________

-> B  : Bidirectional digital with CMOS input		
-> DIO: Digital input output		
-> NP : pdpu = default no-pull with programmable options following the colon (:)		
-> PD : nppu = default pull-down with programmable options following the colon (:)		
-> PU : nppd = default pull-up with programmable options following the colon (:)		
*/

//AON1 - AON_GPIO0 (GPIO20)
#define AON_INSTANCE             1                  /**</ AON pin instance. */
#define AON_GPIO_PIN             4                  /**</ AON pin number. */
#define AON_PAD_ID               31                 /**</ AON Pad ID. */
#define AON_PAD_ALT_FUNC         PAD_MuxAlt0        /**</ AON pin alternate function. */
#define AON_ID                   1                  /**</ AON pin ID (AON1). */

#define AON_ON  1                                   /**</ AON on. */
#define AON_OFF 0                                   /**</ AON off. */

/* Functions  ------------------------------------------------------------------*/


/*!******************************************************************
 * \fn void HT_AON_App(void)
 * \brief AON example main function.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_AON_App(void);

#endif /*__HT_AON_API_H__*/

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
