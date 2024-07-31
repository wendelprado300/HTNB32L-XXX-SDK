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
 * \file HT_ADC_Api.h
 * \brief ADC Thermal sensor. 
 * \author HT Micron Advanced R&D,
 *         Matheus da Silva Zorzeto, Hêndrick Bataglin Gonçalves, Christian Roberto Lehmen, Felipe Kalinski Ferreira,
 *         Leandro Borges, Mauricio Carlotto Ribeiro, Henrique Kuhn, Cleber Haack, Eduardo Mendel
 * 
  * \link https://github.com/htmicron
 * \version 0.1
 * \date March 08, 2024
 */

#ifndef __HT_ADC_DEMO_H_
#define __HT_ADC_DEMO_H_

#include "main.h"
#include "hal_adc.h"
#include "HT_adc_qcx212.h"
#include <stdio.h>
#include "string.h"
#include "adc_qcx212.h"
#include "HT_bsp.h"
#include "stdint.h"

/* Defines  ------------------------------------------------------------------*/

#define DEMO_ADC_CHANNEL  ADC_ChannelAio2        /**</ ADC channel. */

#define MAX_TEMP          85
#define MIN_TEMP          -15 

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn static void HT_ADC_InitThermal(void)
 * \brief Initialize ADC thermal channel.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
void HT_ADC_Thermal_Init(void);

/*!******************************************************************
 * \fn void HT_ADC_Thermal_Read(void)
 * \brief ADC thermal reading function.
 *
 * \param[in]  none
 * \param[out] temp Thermal read
 *
 * \retval none
 *******************************************************************/
int HT_ADC_Thermal_Read(void);

#endif /*__HT_ADC_DEMO_H_*/

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
