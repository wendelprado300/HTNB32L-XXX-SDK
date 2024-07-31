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

#include "HT_ADC_Api.h"

static volatile uint32_t callback = 0;
static volatile uint32_t thermal_result = 0;
static adc_config_t thermalConfig;

/*!******************************************************************
 * \fn static void HT_ADC_ThermalChannelCallback(uint32_t result)
 * \brief ADC Thermal conversion callback. Function that will be called 
 * after the ADC finishes to convert the Thermal channel.
 *
 * \param[in]  uint32_t result              ADC conversion output.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
static void HT_ADC_ThermalChannelCallback(uint32_t result);

/*!******************************************************************
 * \fn static int32_t HT_ADC_GetTemperatureValue(uint32_t ad_value)
 * \brief Calculates the temperature from the respective AD value.
 *
 * \param[in]  uint32_t ad_value              ADC output value.
 * \param[out] none
 *
 * \retval Voltage value calculated.
 *******************************************************************/
static int32_t HT_ADC_GetTemperatureValue(uint32_t ad_value);

/*!******************************************************************
 * \fn static void HT_ADC_InitThermal(void)
 * \brief Initialize ADC thermal channel.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_ADC_InitThermal(void);

static void HT_ADC_ThermalChannelCallback(uint32_t result) {
    callback |= ADC_ChannelThermal;
    thermal_result = result;
}

static int32_t HT_ADC_GetTemperatureValue(uint32_t ad_value) {
    return (int)HAL_ADC_ConvertThermalRawCodeToTemperature(ad_value);
}

static void HT_ADC_InitThermal(void) {
    ADC_GetDefaultConfig(&thermalConfig);

    thermalConfig.channelConfig.thermalInput = ADC_ThermalInputVbat;

    HT_ADC_ChannelInit(ADC_ChannelThermal, ADC_UserAPP, &thermalConfig, HT_ADC_ThermalChannelCallback);
}

void HT_ADC_Thermal_Init(void){
    HT_ADC_InitThermal();
}

int HT_ADC_Thermal_Read(void) {
    int temp;

    do{
        callback = 0;

        HT_ADC_StartConversion(ADC_ChannelThermal, ADC_UserAPP);

        while(callback != ADC_ChannelThermal);

        temp = (int)HT_ADC_GetTemperatureValue(thermal_result);

        delay_us(200000);
    }while( !(temp <= MAX_TEMP) && (temp >= MIN_TEMP) );

    return temp;
}

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
