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
 * \file main.h
 * \brief Driver Example main file. 
  * \author HT Micron Advanced R&D,
 *         Hêndrick Bataglin Gonçalves,
 *         Gleiser Alvarez Arrojo
 * \link https://github.com/htmicron
 * \version 0.1
 * \date February 23, 2023
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include "string.h"
#include "HT_UART_Api.h"
#include "HT_bsp.h"
#include "htnb32lxxx_hal_usart.h"
#include "HT_GPIO_Demo.h"
#include "HT_I2C_Demo.h"
#include "HT_ADC_Demo.h"
#include "HT_SPI_Demo.h"
#include "HT_AON_Demo.h"
#include "HT_WDT_Demo.h"
#include "HT_USART_Demo.h"
#include "stdio.h"

/* Defines  ------------------------------------------------------------------*/

#define HT_DRIVER_EXAMPLE 1

/* Typedefs  -----------------------------------------------------------------*/

/**
 * \enum HT_Peripheral
 * \brief Peripheral examples available in this SDK. User must choose one of
 * these options in order to run a demo app.
 */
typedef enum {
    I2C_EX,
    SPI_EX,
    GPIO_EX,
    AON_EX,
    ADC_EX,
    WDT_EX,
    USART_EX
} HT_Peripheral;

#endif /* __MAIN_H__ */

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
