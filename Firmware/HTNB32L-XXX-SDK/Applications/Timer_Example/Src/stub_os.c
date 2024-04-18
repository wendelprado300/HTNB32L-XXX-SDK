
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

#include <string.h>
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "semphr.h"                     // ARM.FreeRTOS::RTOS:Core
#include "slpman_qcx212.h"
#include "plat_config.h"
#include "hal_uart.h"

#ifndef IS_IRQ_MODE
#define IS_IRQ_MODE()             (__get_IPSR() != 0U)
#endif

__WEAK uint8_t* getDebugDVersion(void) {
    return 0;
}

osStatus_t osSemaphoreAcquire (osSemaphoreId_t semaphore_id, uint32_t timeout) {
    return (osStatus_t)0;    
}

osStatus_t osSemaphoreDelete (osSemaphoreId_t semaphore_id) {
    return (osStatus_t)0;
}

osSemaphoreId_t osSemaphoreNew (uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr) {
    return 0;
}

osStatus_t osSemaphoreRelease (osSemaphoreId_t semaphore_id) {
    return (osStatus_t)0;
}

void PhyDebugPrint(uint8_t moduleID, uint8_t subID, uint32_t *pSwLogData, uint32_t swLogLen, uint8_t uniLogLevel, uint8_t ramLogSwitch) {
    return;
}

char *pcTaskGetName(TaskHandle_t xTaskToQuery) {
    return NULL;
}

TaskHandle_t xTaskGetCurrentTaskHandle(void) {
    return (TaskHandle_t)NULL;
}

osStatus_t osDelay (uint32_t ticks) {
    return (osStatus_t)0;
}

osKernelState_t osKernelGetState (void) {
    return (osKernelState_t)0;
}

uint8_t osThreadIsSuspendAll(void) {
    return 0;
}

uint8_t PhyLogLevelGet(void) {
    return 0;
}

BOOL PHY_GET_UNILOG_ONOFF(void) { 
    return 0;
}

BOOL PhyLogAllowCheck(UINT8 uniLogLevel) {
    return 0;
}

BOOL RF_HW_INFO_IS_DCXO(void) {
    return 0;
}

void RfSpllOutToBcClkEna(void) {
    return;
}

UINT32 SQ_ScLtch_Read(void) {
    return 0;
}

void prvStopTickInterruptTimer(void) {
    return;
}

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
