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

#include "HT_AON_Demo.h"

/*!******************************************************************
 * \fn static void HT_AON_Init(void)
 * \brief Initialize AON1 as output.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_AON_Init(void);

static void HT_AON_Init(void) {
  GPIO_InitType GPIO_InitStruct = {0};

  GPIO_InitStruct.af = PAD_MuxAlt0;
  GPIO_InitStruct.pad_id = AON_PAD_ID;
  GPIO_InitStruct.gpio_pin = AON_GPIO_PIN;
  GPIO_InitStruct.pin_direction = GPIO_DirectionOutput;
  GPIO_InitStruct.init_output = 1;
  GPIO_InitStruct.pull = PAD_AutoPull;
  GPIO_InitStruct.instance = AON_INSTANCE;
  GPIO_InitStruct.exti = GPIO_EXTI_DISABLED;

  HT_GPIO_Init(&GPIO_InitStruct);
}

void HT_AON_App(void) {

  print_uart("AON Example Start!\n");
  
  HT_AON_Init();
  pmuAONIOPowerOn(1);

  slpManNormalIOVoltSet(IOVOLT_3_30V);
  slpManAONIOVoltSet(IOVOLT_3_30V);

  while(1) {

      print_uart("AON1 ON...\n");
      HT_GPIO_WritePin(AON_GPIO_PIN, AON_INSTANCE, AON_ON);
      delay_us(10000000);
      print_uart("AON1 OFF...\n");
      HT_GPIO_WritePin(AON_GPIO_PIN, AON_INSTANCE, AON_OFF);
      delay_us(10000000);
      
  }
}

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
