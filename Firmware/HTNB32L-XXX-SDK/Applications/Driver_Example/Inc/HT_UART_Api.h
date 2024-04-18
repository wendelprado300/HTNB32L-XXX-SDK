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
 * \file HT_UART_Api.h
 * \brief UART API for Driver Example. 
  * \author HT Micron Advanced R&D,
 *         Hêndrick Bataglin Gonçalves,
 *         Gleiser Alvarez Arrojo
 * 
 * \link https://github.com/htmicron
 * \version 0.1
 * \date February 23, 2023
 */

#ifndef __HT_UART_API_H__
#define __HT_UART_API_H__

#include "htnb32lxxx_hal_usart.h"
#include "Driver_USART.h"
#include "bsp.h"
#include "stdarg.h"

/* Defines  ------------------------------------------------------------------*/
#define PRINTF_BUFFER_SIZE   250                  /**</ USART Max buffer size to be used in the printf function. */

/* Functions  ----------------------------------------------------------------*/

/*!******************************************************************
 * \fn void ht_printf(const char *format, ...)
 * \brief Print function. Sends logs through UART TX.
 *
 * \param[in] const char *format                   String to be sent through UART.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void ht_printf(const char *format, ...);

#endif /* __HT_UART_API_H__ */

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
