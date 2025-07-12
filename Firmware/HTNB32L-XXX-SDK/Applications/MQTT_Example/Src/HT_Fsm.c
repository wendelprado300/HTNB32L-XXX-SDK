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

#include "HT_Fsm.h"
#include "hal_adc.h"
#include "HT_adc_qcx212.h"
#include <stdio.h>
#include "string.h"
#include "adc_qcx212.h"
#include "HT_bsp.h"
#include "stdint.h"

// ADC Converter
#define DEMO_ADC_CHANNEL ADC_ChannelAio2        /**</ ADC channel. */
static volatile uint32_t adcCallback = 0;
static volatile uint32_t user_adc_channel = 0;

static adc_config_t adcConfig;

// MQTT
static MQTTClient mqttClient;
static Network mqttNetwork;

//Buffer that will be published.
static uint8_t mqttSendbuf[HT_MQTT_BUFFER_SIZE] = {0};
static uint8_t mqttReadbuf[HT_MQTT_BUFFER_SIZE] = {0};

static const char clientID[] = {"SIP_HTNB32L-XXX"};
static const char username[] = {""};
static const char password[] = {""};

//MQTT broker host address
static const char addr[] = {"131.255.82.115"};

// MQTT Topics to subscribe
char topic_led_1[] = {"hana/prado/htnb32l_led_1"};
char topic_led_2[] = {"hana/prado/htnb32l_led_2"};
char topic_led_3[] = {"hana/prado/htnb32l_led_3"};

// MQTT Topics to Publish
char topic_button_1[] = {"hana/prado/htnb32l_button_1"};
char topic_button_2[] = {"hana/prado/htnb32l_button_2"};
char topic_ldr[] = {"hana/prado/htnb32l_ldr_value"};

//Buffer where the digital twin messages will be stored.
static uint8_t subscribed_payload[HT_SUBSCRIBE_BUFF_SIZE] = {0};
static uint8_t subscribed_topic[255] = {0};
volatile MessageData recieved_msg = {0};


static StaticTask_t yield_thread;
static uint8_t yieldTaskStack[1024*4];
static StaticTask_t led_thread;
static uint8_t ledTaskStack[1024*4];
static StaticTask_t ldr_thread;
static uint8_t ldrTaskStack[1024*4];

//Subcribe callback flag
volatile uint8_t subscribe_callback = 0;

//FSM state.
volatile HT_FSM_States state = HT_WAIT_FOR_BUTTON_STATE;

//Button color definition.
volatile HT_Button button_color = HT_UNDEFINED;

//Button IRQn flag
extern volatile uint8_t button_irqn; 

extern uint16_t blue_irqn_mask;
extern uint16_t white_irqn_mask;

static volatile uint8_t blue_button_state = 0;
static volatile uint8_t white_button_state = 0;

/* Function prototypes  ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn static void HT_ADC_ConversionCallback(uint32_t result)
 * \brief ADC convertion callback. Function that will be called 
 * after the ADC finishes its conversion.
 *
 * \param[in]  uint32_t result              ADC conversion output.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
static void HT_ADC_ConversionCallback(uint32_t result);

/*!******************************************************************
 * \fn static uint32_t HT_ADC_GetVoltageValue(uint32_t ad_value)
 * \brief Calculates the voltage from the respective AD value.
 *
 * \param[in]  uint32_t ad_value              ADC output value.
 * \param[out] none
 *
 * \retval Voltage value calculated.
 *******************************************************************/
static uint32_t HT_ADC_GetVoltageValue(uint32_t ad_value);

/*!******************************************************************
 * \fn static void HT_ADC_Init(uint8_t channel)
 * \brief Initialize ADC user channel.
 *
 * \param[in]  uint8_t channel              ADC channel that will be used.
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_ADC_Init(uint8_t channel);

/*!******************************************************************
 * \fn static void HT_YieldThread(void *arg)
 * \brief Thread created as MQTT background.
 *
 * \param[in]  void *arg                    Thread parameter.
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_YieldThread(void *arg);

/*!******************************************************************
 * \fn static void HT_Yield_Thread_Start(void *arg)
 * \brief Creates a thread that will be the MQTT background.
 *
 * \param[in]  void *arg                    Parameters that will be used in the created thread.
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_Yield_Thread_Start(void *arg);

/*!******************************************************************
 * \fn static void HT_LedThread(void *arg)
 * \brief Thread to monitore subscribed led status
 *
 * \param[in]  void *arg                    Thread parameter.
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_LedThread(void *arg);

/*!******************************************************************
 * \fn static void HT_LdrThread(void *arg)
 * \brief Thread to monitore subscribed led status
 *
 * \param[in]  void *arg                    Thread parameter.
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_LdrThread(void *arg);

/*!******************************************************************
 * \fn static void HT_LdrThread(void *arg)
 * \brief Thread to monitore subscribed led status
 *
 * \param[in]  void *arg                    Thread parameter.
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_BtnThread(void *arg);

/*!******************************************************************
 * \fn static void HT_Led_Thread_Start(void *arg)
 * \brief Creates the thread to monitore the sbscribed led status.
 *
 * \param[in]  void *arg                    Parameters that will be used in the created thread.
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_Led_Thread_Start(void *arg);

/*!******************************************************************
 * \fn static void HT_Ldr_Thread_Start(void *arg)
 * \brief Creates the thread to monitore the sbscribed led status.
 *
 * \param[in]  void *arg                    Parameters that will be used in the created thread.
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_Ldr_Thread_Start(void *arg);

/*!******************************************************************
 * \fn static void HT_Btn_Thread_Start(void *arg)
 * \brief Creates the thread to monitore the sbscribed led status.
 *
 * \param[in]  void *arg                    Parameters that will be used in the created thread.
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_bTN_Thread_Start(void *arg);

/* ---------------------------------------------------------------------------------------*/

static void HT_ADC_ConversionCallback(uint32_t result) {
    adcCallback |= DEMO_ADC_CHANNEL;
    user_adc_channel = result;
}

static uint32_t HT_ADC_GetVoltageValue(uint32_t ad_value) {
    uint32_t value;
    value = HAL_ADC_CalibrateRawCode(ad_value);
    return (uint32_t)(value*16/3);
}

static void HT_ADC_Init(uint8_t channel) {
    ADC_GetDefaultConfig(&adcConfig);

    adcConfig.channelConfig.aioResDiv = ADC_AioResDivRatio3Over16; 

    ADC_ChannelInit(channel, ADC_UserAPP, &adcConfig, HT_ADC_ConversionCallback);
}

static void HT_YieldThread(void *arg) {
    while (1) {
        // Wait function for 10ms to check if some message arrived in subscribed topic
        MQTTYield(&mqttClient, 10);
    }
}

static void HT_LedThread(void *arg) {
    printf("LED Task Started\n");
    while (1) {
        while (!subscribe_callback)
        {
          osDelay(pdMS_TO_TICKS(10));
        }
        subscribe_callback = 0;

        uint16_t state = (uint16_t) atoi((char *)subscribed_payload);

        if (strcmp((char *)subscribed_topic, topic_led_1) == 0)
        {
          HT_GPIO_WritePin(BLUE_LED_PIN, BLUE_LED_INSTANCE, !state);
        }
        else if (strcmp((char *)subscribed_topic, topic_led_2) == 0)
        {
          HT_GPIO_WritePin(WHITE_LED_PIN, WHITE_LED_INSTANCE, !state);
        }
        else if (strcmp((char *)subscribed_topic, topic_led_3) == 0)
        {
          HT_GPIO_WritePin(GREEN_LED_PIN, GREEN_LED_INSTANCE, !state);
        }

        printf("Received Topic: %s, payload: %s\n", subscribed_topic, subscribed_payload);
        
        osDelay(pdMS_TO_TICKS(10));
    }
}

static void HT_LdrThread(void *arg) {
    printf("LDR Task Started\n");
    HT_ADC_Init(DEMO_ADC_CHANNEL);
    while (1) {
        adcCallback = 0;

        HT_ADC_StartConversion(DEMO_ADC_CHANNEL, ADC_UserAPP);

        while(adcCallback != (DEMO_ADC_CHANNEL))
        {
          osDelay(pdMS_TO_TICKS(20));
        }
        uint32_t adcValue = HT_ADC_GetVoltageValue(user_adc_channel);
        uint8_t ldrPayload[20] = {0};
        sprintf((char *)ldrPayload,"%u",adcValue);
        printf("ADC Value: %dmv\n", adcValue);
        printf("Publishing LDR...\n");
        
        // TAREFA EXTRA: Implementar mutex aqui para evitar chamar o HT_MQTT_Publish em dois lugares simultaneamente.
        HT_MQTT_Publish(&mqttClient, topic_ldr, ldrPayload, strlen((char *)ldrPayload), QOS0, 0, 0, 0);
        
        osDelay(pdMS_TO_TICKS(6000));
    }
}

static void HT_BtnThread(void *arg) {
    // IMPLEMENTAR ESSA FUNCAO
}

static void HT_Yield_Thread_Start(void *arg) {
    osThreadAttr_t task_attr;

    memset(&task_attr,0,sizeof(task_attr));
    memset(yieldTaskStack, 0xA5,LED_TASK_STACK_SIZE);
    task_attr.name = "yield_thread";
    task_attr.stack_mem = yieldTaskStack;
    task_attr.stack_size = LED_TASK_STACK_SIZE;
    task_attr.priority = osPriorityNormal;
    task_attr.cb_mem = &yield_thread;
    task_attr.cb_size = sizeof(StaticTask_t);

    osThreadNew(HT_YieldThread, NULL, &task_attr);
}

static void HT_Led_Thread_Start(void *arg) {
    osThreadAttr_t task_attr;

    memset(&task_attr,0,sizeof(task_attr));
    memset(ledTaskStack, 0xA5,LED_TASK_STACK_SIZE);
    task_attr.name = "led_thread";
    task_attr.stack_mem = ledTaskStack;
    task_attr.stack_size = LED_TASK_STACK_SIZE;
    task_attr.priority = osPriorityBelowNormal7;
    task_attr.cb_mem = &led_thread;
    task_attr.cb_size = sizeof(StaticTask_t);

    osThreadNew(HT_LedThread, NULL, &task_attr);
}

static void HT_Ldr_Thread_Start(void *arg) {
    osThreadAttr_t task_attr;

    memset(&task_attr,0,sizeof(task_attr));
    memset(ldrTaskStack, 0xA5,LED_TASK_STACK_SIZE);
    task_attr.name = "ldr_thread";
    task_attr.stack_mem = ldrTaskStack;
    task_attr.stack_size = LED_TASK_STACK_SIZE;
    task_attr.priority = osPriorityBelowNormal6;
    task_attr.cb_mem = &ldr_thread;
    task_attr.cb_size = sizeof(StaticTask_t);

    osThreadNew(HT_LdrThread, NULL, &task_attr);
}

static void HT_Btn_Thread_Start(void *arg) {
    // IMPLEMENTAR ESSA FUNCAO
}

static HT_ConnectionStatus HT_FSM_MQTTConnect(void) {

    // Connect to MQTT Broker using client, network and parameters needded. 
    if(HT_MQTT_Connect(&mqttClient, &mqttNetwork, (char *)addr, HT_MQTT_PORT, HT_MQTT_SEND_TIMEOUT, HT_MQTT_RECEIVE_TIMEOUT,
                (char *)clientID, (char *)username, (char *)password, HT_MQTT_VERSION, HT_MQTT_KEEP_ALIVE_INTERVAL, mqttSendbuf, HT_MQTT_BUFFER_SIZE, mqttReadbuf, HT_MQTT_BUFFER_SIZE)) {
        return HT_NOT_CONNECTED;   
    }

    return HT_CONNECTED;
}

static void HT_FSM_Topic_Subscribe(void) {
    
    HT_MQTT_Subscribe(&mqttClient, topic_led_1, QOS0);
    HT_MQTT_Subscribe(&mqttClient, topic_led_2, QOS0);
    HT_MQTT_Subscribe(&mqttClient, topic_led_3, QOS0);

    printf("Subscribe Done!\n");
}

void HT_FSM_SetSubscribeBuff(MessageData *msg) {
    memset(subscribed_payload, 0, HT_SUBSCRIBE_BUFF_SIZE);
    memset(subscribed_topic, 0, strlen((char *)subscribed_topic));
    memcpy(subscribed_payload, msg->message->payload, msg->message->payloadlen);
    memcpy(subscribed_topic, msg->topicName->lenstring.data, msg->topicName->lenstring.len);
}

void HT_Fsm(void) {

    // Initialize MQTT Client and Connect to MQTT Broker defined in global variables
    if(HT_FSM_MQTTConnect() == HT_NOT_CONNECTED) {
        printf("\n MQTT Connection Error!\n");
        while(1);
    }
    
    printf("MQTT Connection Success!\n");
    
    HT_FSM_Topic_Subscribe();

    HT_Yield_Thread_Start(NULL);

    HT_Led_Thread_Start(NULL);

    HT_Ldr_Thread_Start(NULL);

    HT_Btn_Thread_Start(NULL);

}

/************************ HT Micron Semicondutores S.A *****END OF FILE****/