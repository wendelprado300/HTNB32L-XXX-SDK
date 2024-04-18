/*

  _    _ _______   __  __ _____ _____ _____   ____  _   _
 | |  | |__   __| |  \/  |_   _/ ____|  __ \ / __ \| \ | |
 | |__| |  | |    | \  / | | || |    | |__) | |  | |  \| |
 |  __  |  | |    | |\/| | | || |    |  _  /| |  | | . ` |
 | |  | |  | |    | |  | |_| || |____| | \ \| |__| | |\  |
 |_|  |_|  |_|    |_|  |_|_____\_____|_|  \_\\____/|_| \_|
 =================== Advanced R&D ========================

 Copyright (c) 2024 HT Micron Semicondutores S.A.
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
 * \file HT_Timer_Demo.h
 * \brief Timer app. 
 * \author HT Micron Advanced R&D,
 *         Hêndrick Bataglin Gonçalves,
 *         Matheus da Silva Zorzeto
 * 
 * \link https://github.com/htmicron
 * \version 1.0
 * \date April 16, 2024
 */

#ifndef __HT_TIMER_DEMO__
#define __HT_TIMER_DEMO__

#include "main.h"
#include <stdio.h>
#include <string.h>
#include "ht_bsp.h"
#include "timer_qcx212.h"

/* Defines  ------------------------------------------------------------------*/
#define TIMER_INSTANCE 0          /**</ Timer instance that will be used in this example. */
#define TIMER_N_SECONDS 1         /**</ Timer period to generate an interruption. */

/* Functions  ----------------------------------------------------------------*/

/*!******************************************************************
 * \fn void HT_Timer_App(void)
 * \brief Timer example main function.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_Timer_App(void);

#endif /*__HT_TIMER_DEMO__*/

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
