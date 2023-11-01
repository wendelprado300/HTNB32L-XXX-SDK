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

#include "HT_i2c_qcx212.h"
#include "bsp.h"
#include "HT_Peripheral_Config.h"

static ARM_DRIVER_I2C *i2c_handle;

void HT_I2C_MasterReceive(uint32_t addr, uint8_t *ptr, uint32_t len) {
    i2c_handle->MasterReceive(addr, ptr, len, false);
}

void HT_I2C_MasterTransmit(uint32_t addr, uint8_t *ptr, uint32_t len) {
    i2c_handle->MasterTransmit(addr, ptr, len, false);
}

static void HT_I2C_InitClock(HT_I2C_Type i2c_id) {
    switch (i2c_id) {
    case HT_I2C0:
        GPR_ClockDisable(GPR_I2C0FuncClk);
        GPR_SetClockSrc(GPR_I2C0FuncClk, GPR_I2C0ClkSel_26M);    
        GPR_ClockEnable(GPR_I2C0FuncClk);
        GPR_SWReset(GPR_ResetI2C0Func);
        
        break;
    case HT_I2C1:
        GPR_ClockDisable(GPR_I2C1FuncClk);
        GPR_SetClockSrc(GPR_I2C1FuncClk, GPR_I2C1ClkSel_26M);    
        GPR_ClockEnable(GPR_I2C1FuncClk);
        GPR_SWReset(GPR_ResetI2C1Func);

        break;
    }
}

void HT_I2C_Init(I2C_InitType *i2c) {
    HT_I2C_InitClock(i2c->i2c_id);

    i2c_handle = i2c->hi2c;

    i2c_handle->Initialize(i2c->cb_event);
    i2c_handle->PowerControl(i2c->power_state);

    if(i2c->ctrl & I2C_OWN_ADDRESS_MASK) 
        i2c_handle->Control(ARM_I2C_OWN_ADDRESS, i2c->addr_flag);
    
    if(i2c->ctrl & I2C_BUS_SPEED_MASK) 
        i2c_handle->Control(ARM_I2C_BUS_SPEED, i2c->bus_speed);

    if(i2c->ctrl & I2C_BUS_CLEAR_MASK) 
        i2c_handle->Control(ARM_I2C_BUS_CLEAR, i2c->clear_val);

    //not implemented yet
    if(i2c->ctrl & I2C_ABORT_TRANSFER_MASK) 
        i2c_handle->Control(ARM_I2C_ABORT_TRANSFER, 1);
}
