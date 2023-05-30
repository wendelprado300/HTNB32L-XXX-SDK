/*

  _    _ _______   __  __ _____ _____ _____   ____  _   _
 | |  | |__   __| |  \/  |_   _/ ____|  __ \ / __ \| \ | |
 | |__| |  | |    | \  / | | || |    | |__) | |  | |  \| |
 |  __  |  | |    | |\/| | | || |    |  _  /| |  | | . ` |
 | |  | |  | |    | |  | |_| || |____| | \ \| |__| | |\  |
 |_|  |_|  |_|    |_|  |_|_____\_____|_|  \_\\____/|_| \_|
 =================== Advanced R&D ========================

 Copyright (c) 2023 HT Micron Semicondutors S.A.
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
 * \file HT_ADC_Demo.h
 * \brief ADC demonstration app. 
 * \author HT Micron Advanced R&D
 * \link https://github.com/htmicron
 * \version 0.1
 * \date February 23, 2023
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

#define DEMO_ADC_CHANNEL ADC_ChannelAio2        /**</ ADC channel. */

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void HT_ADC_App(void)
 * \brief ADC example main function.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_ADC_App(void);

#endif /*__HT_ADC_DEMO_H_*/

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
