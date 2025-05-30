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

//#include "HT_USART_Demo.h"
#include "htnb32lxxx_hal_usart.h"

volatile uint32_t blink_interval = 0;  // 0 = desligado, >0 = intervalo em ms
static TaskHandle_t xBlinkTaskHandle = NULL;

extern USART_HandleTypeDef huart1;
static uint8_t rx_buffer_usart_1;

char cmdRxBuffer[255] = {0};
int cmdRxBufferIdx = 0;

volatile uint8_t rx_callback_usart_1 = 0;
volatile uint8_t tx_callback_usart_1 = 0;

extern uint8_t *usart_tx_buffer;
extern uint8_t *usart_rx_buffer;;  // PODE PRECISAR DOS


static uint32_t uart_cntrl = (ARM_USART_MODE_ASYNCHRONOUS | ARM_USART_DATA_BITS_8 | ARM_USART_PARITY_NONE |
                                 ARM_USART_STOP_BITS_1 | ARM_USART_FLOW_CONTROL_NONE);

void HT_USART_Callback(uint32_t event) {
    rx_callback_usart_1 = 1;
}

int Count_time = 0 ;
int Count_task2= 0;

//GPIO10 - BUTTON
#define BUTTON_INSTANCE             0
#define BUTTON_PIN                  10
#define BUTTON_PAD_ID               25
#define BUTTON_PAD_ALT_FUNC         PAD_MuxAlt0

//GPIO3 - LED
#define LED_INSTANCE              0
#define LED_GPIO_PIN              3
#define LED_PAD_ID                14
#define LED_PAD_ALT_FUNC          PAD_MuxAlt0

//*********************/


//GPIO5 - LED CONTROLe VAI UART
#define LED_INSTANCE3              0
#define LED_GPIO_PIN3             5
#define LED_PAD_ID3                16
#define LED_PAD_ALT_FUNC          PAD_MuxAlt0

//GPIO4 - LED CONTROLADO PELO BOTÃO
#define LED_INSTANCE2              0
#define LED_GPIO_PIN2              4
#define LED_PAD_ID2                15
#define LED_PAD_ALT_FUNC          PAD_MuxAlt0


#define LED_ON                    1
#define LED_OFF                   0
#define DEMO_ADC_CHANNEL ADC_ChannelAio2 

static volatile uint32_t callback = 0;
static volatile uint32_t user_adc_channel = 0;

volatile bool button_state = false;
QueueHandle_t xFila;
static adc_config_t adcConfig;

static void HT_ADC_ConversionCallback(uint32_t result) {
    //printf("Adc callback: %d", result);
    callback |= DEMO_ADC_CHANNEL;
    user_adc_channel = result;
}

static void  HT_ADC_Init(uint8_t channel) {

    ADC_GetDefaultConfig(&adcConfig);

    adcConfig.channelConfig.aioResDiv = ADC_AioResDivRatio3Over16; 

    ADC_ChannelInit(channel, ADC_UserAPP, &adcConfig, HT_ADC_ConversionCallback);
}


static void HT_GPIO_InitButton(void) {    ////////////// BOTTTTTTT
    
    GPIO_InitType GPIO_InitStruct = {0};

    GPIO_InitStruct.af = PAD_MuxAlt0;
    GPIO_InitStruct.pad_id = BUTTON_PAD_ID;
    GPIO_InitStruct.gpio_pin = BUTTON_PIN;
    GPIO_InitStruct.pin_direction = GPIO_DirectionInput;
    GPIO_InitStruct.pull = PAD_InternalPullUp;
    GPIO_InitStruct.instance = BUTTON_INSTANCE;
    GPIO_InitStruct.exti = GPIO_EXTI_DISABLED;
    GPIO_InitStruct.interrupt_config = GPIO_InterruptFallingEdge;

    HT_GPIO_Init(&GPIO_InitStruct);
}

static void HT_GPIO_InitLed(int pin, int id) {   ///////////////  LEDDDDD  LDRRR
    GPIO_InitType GPIO_InitStruct = {0};

    GPIO_InitStruct.af = PAD_MuxAlt0;
    GPIO_InitStruct.pad_id = id;
    GPIO_InitStruct.gpio_pin = pin;
    GPIO_InitStruct.pin_direction = GPIO_DirectionOutput;
    GPIO_InitStruct.init_output = 0;
    GPIO_InitStruct.pull = PAD_AutoPull;
    GPIO_InitStruct.instance = LED_INSTANCE;
    GPIO_InitStruct.exti = GPIO_EXTI_DISABLED;

    HT_GPIO_Init(&GPIO_InitStruct);

    GPIO_InitStruct.af = PAD_MuxAlt0;
    GPIO_InitStruct.pad_id = LED_PAD_ID2;
    GPIO_InitStruct.gpio_pin = LED_GPIO_PIN2;
    GPIO_InitStruct.pin_direction = GPIO_DirectionOutput;
    GPIO_InitStruct.init_output = 0;
    GPIO_InitStruct.pull = PAD_AutoPull;
    GPIO_InitStruct.instance = LED_INSTANCE2;
    GPIO_InitStruct.exti = GPIO_EXTI_DISABLED;

    HT_GPIO_Init(&GPIO_InitStruct);

    GPIO_InitStruct.af = PAD_MuxAlt0;
    GPIO_InitStruct.pad_id = LED_PAD_ID3;
    GPIO_InitStruct.gpio_pin = LED_GPIO_PIN3;
    GPIO_InitStruct.pin_direction = GPIO_DirectionOutput;
    GPIO_InitStruct.init_output = 0;
    GPIO_InitStruct.pull = PAD_AutoPull;
    GPIO_InitStruct.instance = LED_INSTANCE3;
    GPIO_InitStruct.exti = GPIO_EXTI_DISABLED;

    HT_GPIO_Init(&GPIO_InitStruct);



}

void Task0(void *pvParameters) {
    while (1) {
        callback = 0;
        HT_ADC_StartConversion(DEMO_ADC_CHANNEL, ADC_UserAPP);

        while (callback != DEMO_ADC_CHANNEL) {
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        uint16_t adcValue = user_adc_channel;
        //printf("ADC Value: %d\n", adcValue);

        if (adcValue > 200) {
            HT_GPIO_WritePin(LED_GPIO_PIN, LED_INSTANCE, LED_OFF);
        } else {
            HT_GPIO_WritePin(LED_GPIO_PIN, LED_INSTANCE, LED_ON);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void Task1(void *pvParameters) {
    int button_state = 0;
    int last_button_state = 0;
    BaseType_t xStatus;
    
    while (1) {
        // Lê o estado do botão (invertido por causa do pull-up)
        button_state = !HT_GPIO_PinRead(BUTTON_INSTANCE, BUTTON_PIN);
        
    
        if (button_state ==1 && last_button_state == 0) {
             //uint8_t comando = LED_ON;
            printf("Botão pressionado\n");
            Count_time = 0;
        }
     
        if (button_state == 1){
            Count_time += 10; 
        }
        if (button_state == 0 && last_button_state == 1){
            // botão solto e envia os dados
            printf("tempo: %d ms\n",Count_time); 
            xQueueSend(xFila,&Count_time,0);
        }
        
        last_button_state = button_state;
        vTaskDelay(pdMS_TO_TICKS(10));
        
    }
         

}

void Task2(void *pvParameters) {
    int comandoRecebido;
    BaseType_t xStatus;
    
    while (1) {
        // Espera por um comando na fila (timeout máximo)
        xStatus = xQueueReceive(xFila, &comandoRecebido, portMAX_DELAY);
                printf(xStatus);

                HT_GPIO_WritePin(LED_GPIO_PIN2, LED_INSTANCE2, LED_ON); 
                printf("tempo recebido na task 2 : %d ms\n",comandoRecebido); 
                printf("tempo recebido na task 2 : %d ms\n",Count_time);
                Count_task2 = Count_time;
                vTaskDelay(pdMS_TO_TICKS(comandoRecebido));
                HT_GPIO_WritePin(LED_GPIO_PIN2, LED_INSTANCE2, LED_OFF);
                //vTaskDelay(pdMS_TO_TICKS(2000));

                printf("LED OFF\n");
            }
        }
    
/*void Task3(void *pvParameters) {
    while(1) {
        // Recebe um caractere via UART (bloqueante)
        HAL_USART_ReceivePolling(&huart1, &rx_buffer_usart_1, 1);

        // Armazena no buffer e verifica fim de comando
        cmdRxBuffer[cmdRxBufferIdx++] = rx_buffer_usart_1;
        
        if (rx_buffer_usart_1 == '\r') {
            // Processa o comando recebido
            if (strstr(cmdRxBuffer, "LED_1s") != NULL) {
                while(1){
                HT_GPIO_WritePin(LED_GPIO_PIN3, LED_INSTANCE3, LED_ON);
                vTaskDelay(pdMS_TO_TICKS(1000));
                HT_GPIO_WritePin(LED_GPIO_PIN3, LED_INSTANCE3, LED_OFF);
                vTaskDelay(pdMS_TO_TICKS(1000));
                printf("LED5 1 seg ligado via UART\n");
                }
                
            } 
            else if (strstr(cmdRxBuffer, "LED_2s") != NULL) {
                 while(1){
                 HT_GPIO_WritePin(LED_GPIO_PIN3, LED_INSTANCE3, LED_ON);
                vTaskDelay(pdMS_TO_TICKS(2000));
                HT_GPIO_WritePin(LED_GPIO_PIN3, LED_INSTANCE3, LED_OFF);
                vTaskDelay(pdMS_TO_TICKS(2000));
                printf("LED5 2  UART\n");
                 }
            }
            
            // Limpa buffer para próximo comando
            memset(cmdRxBuffer, 0, sizeof(cmdRxBuffer));
            cmdRxBufferIdx = 0;
        }
    }
}*/

void Task3(void *pvParameters) {
    while(1) {
        // Recebe um caractere via UART (bloqueante)
        HAL_USART_ReceivePolling(&huart1, &rx_buffer_usart_1, 1);

        // Armazena no buffer e verifica fim de comando
        cmdRxBuffer[cmdRxBufferIdx++] = rx_buffer_usart_1;
        
        if (rx_buffer_usart_1 == '\r') {
            // Processa o comando recebido
            if (strstr(cmdRxBuffer, "LED_1s") != NULL) {
                blink_interval = 1000;  // 1 segundo
                printf("Blink 1s ativado\n");
            } 
            else if (strstr(cmdRxBuffer, "LED_2s") != NULL) {
                blink_interval = 2000;  // 2 segundos
                printf("Blink 2s ativado\n");
            }
            else if (strstr(cmdRxBuffer, "LED_off") != NULL) {
                blink_interval = 0;  // Desliga o LED
                HT_GPIO_WritePin(LED_GPIO_PIN3, LED_INSTANCE3, LED_OFF);
                printf("LED desligado\n");
            }
            
            // Limpa buffer para próximo comando
            memset(cmdRxBuffer, 0, sizeof(cmdRxBuffer));
            cmdRxBufferIdx = 0;
        }
    }
}


void Task4(void *pvParameters) {
    while(1) {
        if (blink_interval > 0) {
           
            printf("Blink TASK 4 n");
            HT_GPIO_WritePin(LED_GPIO_PIN3, LED_INSTANCE3, LED_ON);
            vTaskDelay(pdMS_TO_TICKS(blink_interval));  
            printf("AI SIM  \n");
        
            HT_GPIO_WritePin(LED_GPIO_PIN3, LED_INSTANCE3, LED_OFF);
            vTaskDelay(pdMS_TO_TICKS(blink_interval));
            
        }
        else {
        
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

void main_entry(void) {
    BSP_CommonInit();
    slpManNormalIOVoltSet(IOVOLT_3_30V);
    HAL_USART_InitPrint(&huart1, GPR_UART1ClkSel_26M, uart_cntrl, 115200);
    printf("Exemplo FreeRTOS \n");
    
    HT_GPIO_InitLed(LED_GPIO_PIN, LED_PAD_ID);
    HT_GPIO_InitButton();                     
    HT_ADC_Init(DEMO_ADC_CHANNEL);            

    xFila = xQueueCreate(5, sizeof(int));
    if (xFila != NULL) {
        xTaskCreate(Task0, "Task0", 256, NULL, 1, NULL); 
        xTaskCreate(Task1, "Task1", 256, NULL, 2, NULL); 
        xTaskCreate(Task2, "Task2", 256, NULL, 1, NULL);
        xTaskCreate(Task3, "Task3", 256, NULL, 1, NULL);
        xTaskCreate(Task4, "Task4", 256, NULL, 3, NULL);

        vTaskStartScheduler();
    }

    printf("Nao deve chegar aqui.\n");
    while(1);
}