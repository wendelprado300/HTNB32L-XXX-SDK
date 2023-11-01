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
 * \file HT_i2c_qcx212.h
 * \brief I2C API for HTNB32L.
  * \author HT Micron Advanced R&D,
 *         Hêndrick Bataglin Gonçalves
 * 
 * \link https://github.com/htmicron
 * \version 1.0
 * \date October 31, 2023
 */

#ifndef __HT_I2C_QCX212_H__
#define __HT_I2C_QCX212_H__

#include <stdio.h>
#include "HT_Peripheral_Config.h"
#include "HT_bsp.h"

#define I2C_OWN_ADDRESS_MASK (1 << 0)
#define I2C_BUS_SPEED_MASK  (1 << 1)
#define I2C_BUS_CLEAR_MASK (1 << 2)
#define I2C_ABORT_TRANSFER_MASK (1 << 3)

typedef enum {
    HT_I2C0 = 0,
    HT_I2C1
} HT_I2C_Type;

typedef struct {
    ARM_DRIVER_I2C *hi2c;
    HT_I2C_Type i2c_id;
    ARM_I2C_SignalEvent_t cb_event;
    ARM_POWER_STATE power_state;
    ARM_POWER_STATE state;
    uint32_t ctrl;
    uint32_t bus_speed;
    uint32_t clear_val;
    uint32_t addr_flag;
} I2C_InitType;

void HT_I2C_Init(I2C_InitType *i2c);
void HT_I2C_MasterTransmit(uint32_t addr, uint8_t *ptr, uint32_t len);
void HT_I2C_MasterReceive(uint32_t addr, uint8_t *ptr, uint32_t len);

#endif /*__HT_I2C_QCX212_H__*/

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
