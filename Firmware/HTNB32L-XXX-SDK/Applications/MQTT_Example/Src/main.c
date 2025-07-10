#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "slpman_qcx212.h"
#include "pad_qcx212.h"
#include "HT_gpio_qcx212.h"
#include "ic_qcx212.h"
#include "HT_ic_qcx212.h"
#include "hal_adc.h"
#include "HT_adc_qcx212.h"
#include <stdio.h>
#include "string.h"
#include "adc_qcx212.h"
#include "HT_bsp.h"
#include "stdint.h"
#include "htnb32lxxx_hal_usart.h"
#include "HT_Peripheral_Config.h"
#include "HT_usart_unilog.h"
#include <stdbool.h> 

#include "HT_Fsm.h" 

extern USART_HandleTypeDef huart1;
extern uint32_t SystemCoreClock;


// =================================================================
// ===================== MQTT PARAMETROS  =====================
// =================================================================

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

// MQTT Topics to Publish
char topic_Temperatura[] = {"hana/mesanino/senseclima/01/Temperature"};
char topic_Humidade[] = {"hana/mesanino/senseclima/01/Humidade"};



static HT_ConnectionStatus HT_FSM_MQTTConnect(void) {
    // Conecta ao broker MQTT
    if(HT_MQTT_Connect(&mqttClient, &mqttNetwork, (char *)addr, HT_MQTT_PORT, HT_MQTT_SEND_TIMEOUT, HT_MQTT_RECEIVE_TIMEOUT,
                (char *)clientID, (char *)username, (char *)password, HT_MQTT_VERSION, HT_MQTT_KEEP_ALIVE_INTERVAL, 
                mqttSendbuf, HT_MQTT_BUFFER_SIZE, mqttReadbuf, HT_MQTT_BUFFER_SIZE)) {
        return HT_NOT_CONNECTED;   
    }
    return HT_CONNECTED;
}

void HT_Fsm(void) {

    // Initialize MQTT Client and Connect to MQTT Broker defined in global variables
    if(HT_FSM_MQTTConnect() == HT_NOT_CONNECTED) {
        printf("\n MQTT Connection Error!\n");
        while(1);
    }
}




// =================================================================
// ===================== DEFINIÇÕES E VARIÁVEIS =====================
// =================================================================

// Definições para o pino do DHT22 
#define DHT22_INSTANCE              0
#define DHT22_PIN                   10
#define DHT22_PAD_ID                25
#define DHT22_PAD_ALT_FUNC          PAD_MuxAlt0

// Tamanhos das stacks das threads
#define DHT22_THREAD_STACK_SIZE     512
#define MQTT_THREAD_STACK_SIZE      512

// Fila para comunicação entre threads
QueueHandle_t xSensorDataQueue;

// Estrutura para os dados do sensor
typedef struct {
    float temperature;
    float humidity;
} SensorData_t;

// =================================================================
// ===================== FUNÇÕES DO DHT22 ==========================
// =================================================================

void precise_delay_us(uint32_t us) {
    uint32_t cycles_per_us = SystemCoreClock / 1000000 / 8;
    for (volatile uint32_t i = 0; i < us * cycles_per_us; ++i) {
        __asm__("nop");
    }
}

static void dht22_pin_as_output() {
    GPIO_InitType GPIO_InitStruct = {0};
    GPIO_InitStruct.af = DHT22_PAD_ALT_FUNC;
    GPIO_InitStruct.pad_id = DHT22_PAD_ID;
    GPIO_InitStruct.gpio_pin = DHT22_PIN;
    GPIO_InitStruct.pin_direction = GPIO_DirectionOutput;
    GPIO_InitStruct.init_output = 1; 
    GPIO_InitStruct.pull = PAD_AutoPull;
    GPIO_InitStruct.instance = DHT22_INSTANCE;
    HT_GPIO_Init(&GPIO_InitStruct);
}

static void dht22_pin_as_input() {
    GPIO_InitType GPIO_InitStruct = {0};
    GPIO_InitStruct.af = DHT22_PAD_ALT_FUNC;
    GPIO_InitStruct.pad_id = DHT22_PAD_ID;
    GPIO_InitStruct.gpio_pin = DHT22_PIN;
    GPIO_InitStruct.pin_direction = GPIO_DirectionInput;
    GPIO_InitStruct.pull = PAD_InternalPullUp;
    GPIO_InitStruct.instance = DHT22_INSTANCE;
    HT_GPIO_Init(&GPIO_InitStruct);
}

bool dht22_read(float* humidity, float* temperature) {
    uint8_t data[5] = {0, 0, 0, 0, 0};
    uint32_t timeout_counter;

    __disable_irq(); 

    // 1. Envia o sinal de início
    dht22_pin_as_output();
    HT_GPIO_WritePin(DHT22_PIN, DHT22_INSTANCE, 0); // LOW
    precise_delay_us(18000); // 18ms

    HT_GPIO_WritePin(DHT22_PIN, DHT22_INSTANCE, 1); // HIGH
    dht22_pin_as_input();
    precise_delay_us(40);

    // 2. Aguarda a resposta do sensor
    timeout_counter = 10000;
    while(HT_GPIO_PinRead(DHT22_INSTANCE, DHT22_PIN) == 1) { 
        if (timeout_counter-- == 0) { 
            __enable_irq(); 
            return false; 
        }
    }

    timeout_counter = 10000;
    while(HT_GPIO_PinRead(DHT22_INSTANCE, DHT22_PIN) == 0) { 
        if (timeout_counter-- == 0) { 
            __enable_irq(); 
            return false; 
        }
    }

    timeout_counter = 10000;
    while(HT_GPIO_PinRead(DHT22_INSTANCE, DHT22_PIN) == 1) { 
        if (timeout_counter-- == 0) { 
            __enable_irq(); 
            return false; 
        }
    }

    // 3. Lê os 40 bits de dados
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 8; i++) {
            timeout_counter = 10000;
            while(HT_GPIO_PinRead(DHT22_INSTANCE, DHT22_PIN) == 0) { 
                if (timeout_counter-- == 0) { 
                    __enable_irq(); 
                    return false; 
                }
            }

            precise_delay_us(35);
            
            data[j] <<= 1;
            if (HT_GPIO_PinRead(DHT22_INSTANCE, DHT22_PIN) == 1) {
                data[j] |= 1;
                timeout_counter = 10000;
                while(HT_GPIO_PinRead(DHT22_INSTANCE, DHT22_PIN) == 1) { 
                    if (timeout_counter-- == 0) { 
                        __enable_irq(); 
                        return false; 
                    }
                }
            }
        }
    }

    __enable_irq();

    // 4. Verifica o Checksum
    uint8_t checksum = (data[0] + data[1] + data[2] + data[3]) & 0xFF;
    if (checksum != data[4]) {
        return false;
    }

    // 5. Decodifica os dados
    *humidity = ((data[0] << 8) | data[1]) / 10.0f;
    *temperature = (((data[2] & 0x7F) << 8) | data[3]) / 10.0f;
    if (data[2] & 0x80) {
        *temperature *= -1;
    }

    return true;
}

// =================================================================
// ===================== THREAD DE LEITURA DHT22 ===================
// =================================================================

void dht22_thread(void *pvParameters) {
    SensorData_t sensorData;
    
    // Inicializa o pino do DHT22
    dht22_pin_as_input();
    vTaskDelay(pdMS_TO_TICKS(2000)); // Espera inicial para estabilização
    
    while(1) {
        if (dht22_read(&sensorData.humidity, &sensorData.temperature)) {
            // Envia os dados para a fila
            if (xQueueSend(xSensorDataQueue, &sensorData, portMAX_DELAY) != pdPASS) {
                printf("Erro ao enviar dados para a fila!\n");
            }
        } else {
            printf("Falha na leitura do DHT22\n");
        }
        
        // Espera 3 segundos antes da próxima leitura
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

// =================================================================
// ===================== THREAD DE ENVIO MQTT ======================
// =================================================================

void mqtt_thread(void *pvParameters) {
    SensorData_t receivedData;
    char tempPayload[20]; // Buffer para temperatura
    char humPayload[20];  // Buffer para umidade
    
    while(1) {
        // Aguarda dados da fila (bloqueante)
        if (xQueueReceive(xSensorDataQueue, &receivedData, portMAX_DELAY) == pdPASS) {
            // Formata os dados para impressão (simulando envio MQTT)
            
            snprintf(tempPayload, sizeof(tempPayload), "%.1f", receivedData.temperature);
            snprintf(humPayload, sizeof(humPayload), "%.1f", receivedData.humidity);
           
            
            HT_MQTT_Publish(&mqttClient, "hana/mesanino/senseclima/01/Temperature", (uint8_t *)tempPayload, 
                          strlen(tempPayload), QOS0, 0, 0, 0);
                         
            HT_MQTT_Publish(&mqttClient, "hana/mesanino/senseclima/01/Humidade", (uint8_t *)humPayload, 
                          strlen(humPayload), QOS0, 0, 0, 0);
             printf("Dados enviados: Temp=%s°C, Umidade=%s%%\n", tempPayload, humPayload);              
        }
    }
}

// =================================================================
// ===================== FUNÇÃO PRINCIPAL ==========================
// =================================================================

void main_entry(void) {
    // Inicialização básica do sistema
    
    BSP_CommonInit();
    slpManNormalIOVoltSet(IOVOLT_3_30V);
    void HT_Fsm(void);
    
    
    // Configuração da UART para logging
    uint32_t uart_ctrl = (ARM_USART_MODE_ASYNCHRONOUS | ARM_USART_DATA_BITS_8 | 
                          ARM_USART_PARITY_NONE | ARM_USART_STOP_BITS_1 | 
                          ARM_USART_FLOW_CONTROL_NONE);
    HAL_USART_InitPrint(&huart1, GPR_UART1ClkSel_26M, uart_ctrl, 115200);
    
    printf(">>> Sistema com Threads para DHT22 e MQTT <<<\n");
    printf("System Core Clock: %lu Hz\n", SystemCoreClock);

    // 1. Estabelece conexão MQTT primeiro
    HT_Fsm();
    
    // Cria a fila para comunicação entre threads
    xSensorDataQueue = xQueueCreate(5, sizeof(SensorData_t));
    if (xSensorDataQueue == NULL) {
        printf("Erro ao criar a fila de dados!\n");
        while(1);
    }
    
    
    // Cria as threads
    xTaskCreate(dht22_thread, "DHT22_Thread", DHT22_THREAD_STACK_SIZE, NULL, 3, NULL);
    xTaskCreate(mqtt_thread, "MQTT_Thread", MQTT_THREAD_STACK_SIZE, NULL, 2, NULL);
    
    // Inicia o escalonador do FreeRTOS
    vTaskStartScheduler();


    // Se chegou aqui, algo deu errado
    while(1) {
        printf("Erro no escalonador do FreeRTOS!\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}