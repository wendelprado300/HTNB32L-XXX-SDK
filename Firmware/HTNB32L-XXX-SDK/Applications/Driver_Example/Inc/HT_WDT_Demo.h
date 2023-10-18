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
 * \file HT_WDT_Demo.h
 * \brief Watchdog demonstration app. 
  * \author HT Micron Advanced R&D,
 *         Hêndrick Bataglin Gonçalves,
 *         Matheus Zorzeto
 * 
 * \link https://github.com/htmicron
 * \version 1.0
 * \date October 16, 2023
 */

#ifndef __HT_WDT_DEMO_H__
#define __HT_WDT_DEMO_H__

#include "main.h"
#include <stdio.h>
#include <string.h>
#include "ht_bsp.h"
#include "timer_qcx212.h"
#include "wdt_qcx212.h"

/* Defines  ------------------------------------------------------------------*/

/** \brief WDT counter refresh times */
#define WDT_KICK_TIMES      (5)
#define str_size            (35)

/* Functions  ----------------------------------------------------------------*/

/*!******************************************************************
 * \fn void HT_WDT_App(void)
 * \brief WDT example main function.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_WDT_App(void);

#endif /*__HT_WDT_DEMO_H__*/

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
