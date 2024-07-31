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
 * \file HT_BSP_Custom.h
 * \brief Custom settings for iMCP HTNB32L-XXX. 
  * \author HT Micron Advanced R&D,
 *         Hêndrick Bataglin Gonçalves, Christian Roberto Lehmen,  Matheus da Silva Zorzeto, Felipe Kalinski Ferreira,
 *         Leandro Borges, Mauricio Carlotto Ribeiro, Henrique Kuhn, Cleber Haack, Eduardo Mendel
 *         Gleiser Alvarez Arrojo
 * 
 * \link https://github.com/htmicron
 * \version 0.1
 * \date March 15, 2023
 */

#ifndef  __HT_BSP_CUSTOM_H__
#define  __HT_BSP_CUSTOM_H__

#include <stdio.h>
#include <stdarg.h>
#include "clock_qcx212.h"
#include "unilog_qcx212.h"
#include "slpman_qcx212.h"
#include "plat_config.h"
#include "debug_log.h"
#include "bsp.h"

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void BSP_CustomInit(void)
 * \brief Function responsible for startup configurations, such as 
 * UART0 init, MPU init, wakeup pins and retrive platform configuration 
 * from raw flash
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void BSP_CustomInit(void);

/*!******************************************************************
 * \fn void HT_BSP_Init(void)
 * \brief Initializes the core features, such as clock system and pin
 * drivers.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_BSP_Init(void);

/*!******************************************************************
 * \fn void HT_BSP_Deinit(void)
 * \brief Deinitialize the UART peripheral configured on BSP_CustomInit.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_BSP_Deinit(void);

#endif /* __HT_BSP_CUSTOM_H__ */

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
