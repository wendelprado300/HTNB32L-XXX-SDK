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


#include <stdio.h>
#include "bsp.h"
 
#include "main.h"
#include "pad_qcx212.h"
#include "HT_gpio_qcx212.h"
#include "ic_qcx212.h"
#include "HT_ic_qcx212.h"
#include "htnb32lxxx_hal_usart.h"

#include "HT_bsp.h"
#include <stdio.h>
#include <stdbool.h> 

#include "htnb32lxxx_hal_usart.h"
#include "slpman_qcx212.h"
#include "HT_Peripheral_Config.h"
#include "HT_usart_unilog.h"
extern USART_HandleTypeDef huart1;
// =================================================================
// ============== INÍCIO DO CÓDIGO PARA O SENSOR DHT22 ==============
// =================================================================

// Definições para o pino do DHT22 
#define DHT22_INSTANCE              0
#define DHT22_PIN                   10
#define DHT22_PAD_ID                25
#define DHT22_PAD_ALT_FUNC          PAD_MuxAlt0

// Variável externa do sistema que contém o clock da CPU em Hz
extern uint32_t SystemCoreClock;


void precise_delay_us(uint32_t us) {
   
    uint32_t cycles_per_us = SystemCoreClock / 1000000 / 8;
    for (volatile uint32_t i = 0; i < us * cycles_per_us; ++i) {
        __asm__("nop"); // 
    }
}

/**
 * @brief Cria um delay em milissegundos.
 * @param ms Número de milissegundos para esperar.
 */
void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms; ++i) {
        precise_delay_us(1000);
    }
}

// Funções auxiliares para reconfigurar o pino do DHT22
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

/**
 * @brief Lê os dados do sensor DHT22 de forma robusta com timeouts.
 * @param humidity Ponteiro para a variável float que armazenará a umidade.
 * @param temperature Ponteiro para a variável float que armazenará a temperatura.
 * @return true se a leitura foi bem-sucedida, false caso contrário.
 */
bool dht22_read(float* humidity, float* temperature) {
    uint8_t data[5] = {0, 0, 0, 0, 0};
    uint32_t timeout_counter;

    __disable_irq(); 

    // 1. Envia o sinal de início
    dht22_pin_as_output();
    HT_GPIO_WritePin(DHT22_PIN, DHT22_INSTANCE, 0); // LOW
    delay_ms(18); // Delay de 18ms

    HT_GPIO_WritePin(DHT22_PIN, DHT22_INSTANCE, 1); // HIGH
    dht22_pin_as_input();
    precise_delay_us(40);

    // 2. Aguarda a resposta do sensor (com timeout)
    timeout_counter = 10000;
    while(HT_GPIO_PinRead(DHT22_INSTANCE, DHT22_PIN) == 1) { if (timeout_counter-- == 0) { __enable_irq(); return false; } }

    timeout_counter = 10000;
    while(HT_GPIO_PinRead(DHT22_INSTANCE, DHT22_PIN) == 0) { if (timeout_counter-- == 0) { __enable_irq(); return false; } }

    timeout_counter = 10000;
    while(HT_GPIO_PinRead(DHT22_INSTANCE, DHT22_PIN) == 1) { if (timeout_counter-- == 0) { __enable_irq(); return false; } }

    // 3. Lê os 40 bits de dados (com timeouts)
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 8; i++) {
            timeout_counter = 10000;
            while(HT_GPIO_PinRead(DHT22_INSTANCE, DHT22_PIN) == 0) { if (timeout_counter-- == 0) { __enable_irq(); return false; } }

            precise_delay_us(35);
            
            data[j] <<= 1;
            if (HT_GPIO_PinRead(DHT22_INSTANCE, DHT22_PIN) == 1) {
                data[j] |= 1;
                timeout_counter = 10000;
                while(HT_GPIO_PinRead(DHT22_INSTANCE, DHT22_PIN) == 1) { if (timeout_counter-- == 0) { __enable_irq(); return false; } }
            }
        }
    }

    __enable_irq(); // Reabilita as interrupções globais

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

// Função principal de entrada
void main_entry(void) {
    
    BSP_CommonInit();
    slpManNormalIOVoltSet(IOVOLT_3_30V);
    uint32_t uart_cntrl = (ARM_USART_MODE_ASYNCHRONOUS | ARM_USART_DATA_BITS_8 | ARM_USART_PARITY_NONE |
                           ARM_USART_STOP_BITS_1 | ARM_USART_FLOW_CONTROL_NONE);
    HAL_USART_InitPrint(&huart1, GPR_UART1ClkSel_26M, uart_cntrl, 115200);

  

    printf(">>> Sistema Bare-Metal DHT22 Iniciado <<<\n");
    printf("System Core Clock: %lu Hz\n", SystemCoreClock);

    // Inicializa o pino do DHT22 como entrada para um estado seguro
    dht22_pin_as_input();
    delay_ms(2000); // Delay inicial para o sensor estabilizar

   
    while(1) {
        while(1) {
        float humidity = 0.0f;
        float temperature = 0.0f;

       
        if (dht22_read(&humidity, &temperature)) {
            

            // Separa a parte inteira e a fracionária para imprimir
            int temp_int = (int)temperature;
            int temp_frac = (int)((temperature - temp_int) * 10);

            int hum_int = (int)humidity;
            int hum_frac = (int)((humidity - hum_int) * 10);

            // Garante que a parte fracionária seja sempre positiva para a impressão
            if (temp_frac < 0) temp_frac = -temp_frac;
            if (hum_frac < 0) hum_frac = -hum_frac;

            printf("Leitura OK -> Temp: %d.%d C, Umidade: %d.%d %%\n", temp_int, temp_frac, hum_int, hum_frac);

        } else {
         
            printf("!!! Falha ao ler o sensor DHT22 !!!\n");
        }

        // Espera antes da próxima leitura
        delay_ms(3000);
    }
    }
}