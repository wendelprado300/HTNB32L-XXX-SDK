
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

#include "HT_UART_Api.h"
#include "htnb32lxxx_hal_usart.h"

extern USART_HandleTypeDef huart1;

void ht_printf(const char *format, ...) {
    char buffer[PRINTF_BUFFER_SIZE] = {0};

    va_list args;
    
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);  // Store formatted string in buffer
    va_end(args);
    
    HAL_USART_SendPolling(&huart1, (uint8_t *)buffer, strlen(buffer));
}

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
