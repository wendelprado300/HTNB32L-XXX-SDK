/**
 *
 * Copyright (c) 2023 HT Micron Semicondutors S.A.
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

#include "main.h"

void main_entry (void) {
    HT_BSP_Init();
	
    setvbuf(stdout,NULL,_IONBF,0);
	
    PmuEnable(true);
    HT_PmuAnaSettings();

    uniLogFlushOut(0);

    HT_App();

    while(1);

}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
