#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"  // Adicionado para suporte a semáforos
#include "slpman_qcx212.h"
#include "pad_qcx212.h"
#include "HT_gpio_qcx212.h"
#include "ic_qcx212.h"
#include "HT_ic_qcx212.h"

// Definições de hardware (mantidas como no seu código original)
static uint32_t uart_cntrl = (ARM_USART_MODE_ASYNCHRONOUS | ARM_USART_DATA_BITS_8 | ARM_USART_PARITY_NONE | 
                             ARM_USART_STOP_BITS_1 | ARM_USART_FLOW_CONTROL_NONE);

extern USART_HandleTypeDef huart1;

#define BUTTON_INSTANCE          0
#define BUTTON_PIN               10
#define BUTTON_PAD_ID            25
#define BUTTON_PAD_ALT_FUNC      PAD_MuxAlt0

#define LED_INSTANCE             0
#define LED_GPIO_PIN             3
#define LED_PAD_ID               14
#define LED_PAD_ALT_FUNC         PAD_MuxAlt0

#define LED_ON  1
#define LED_OFF 0

// Semáforo para sincronização
SemaphoreHandle_t xSemaforo;

// Protótipos das funções
static void HT_GPIO_InitButton(void);
static void HT_GPIO_InitLed(void);
void Task1(void *pvParameters);
void Task2(void *pvParameters);

static void HT_GPIO_InitButton(void) {
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

static void HT_GPIO_InitLed(void) {
    GPIO_InitType GPIO_InitStruct = {0};
    GPIO_InitStruct.af = PAD_MuxAlt0;
    GPIO_InitStruct.pad_id = LED_PAD_ID;
    GPIO_InitStruct.gpio_pin = LED_GPIO_PIN;
    GPIO_InitStruct.pin_direction = GPIO_DirectionOutput;
    GPIO_InitStruct.init_output = 0;
    GPIO_InitStruct.pull = PAD_AutoPull;
    GPIO_InitStruct.instance = LED_INSTANCE;
    GPIO_InitStruct.exti = GPIO_EXTI_DISABLED;
    HT_GPIO_Init(&GPIO_InitStruct);
}

void Task1(void *pvParameters) {
    bool button_state = false;
    bool last_button_state = false;
    
    while (1) {
        // Lê o estado do botão (invertido por causa do pull-up)
        button_state = (bool) !HT_GPIO_PinRead(BUTTON_INSTANCE, BUTTON_PIN);
        
        // Detecta borda de descida (quando o botão é pressionado)
        if (button_state && !last_button_state) {
            // Libera o semáforo
            xSemaphoreGive(xSemaforo);
            printf("Botão pressionado - semáforo liberado\n");
        }
        
        last_button_state = button_state;
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void Task2(void *pvParameters) {
    while (1) {
        // Espera pelo semáforo (timeout máximo)
        if (xSemaphoreTake(xSemaforo, portMAX_DELAY)) {
            // Acende o LED por 500ms quando recebe o semáforo
            HT_GPIO_WritePin(LED_GPIO_PIN, LED_INSTANCE, LED_ON);
            printf("LED ON\n");
            vTaskDelay(pdMS_TO_TICKS(500));
            HT_GPIO_WritePin(LED_GPIO_PIN, LED_INSTANCE, LED_OFF);
            printf("LED OFF\n");
        }
    }
}

void main_entry(void) {
    // Inicializa hardware
    HT_GPIO_InitButton();
    HT_GPIO_InitLed();
    slpManNormalIOVoltSet(IOVOLT_3_30V);
    
    // Inicializa UART para debug
    HAL_USART_InitPrint(&huart1, GPR_UART1ClkSel_26M, uart_cntrl, 115200);
    printf("Exemplo FreeRTOS com Semáforo\n");
    
    // Cria o semáforo binário
    xSemaforo = xSemaphoreCreateBinary();
    
    if (xSemaforo == NULL) {
        printf("Erro ao criar o semáforo!\n");
        while(1);
    }
    
    // Inicialmente o semáforo está "vazio"
    xSemaphoreTake(xSemaforo, 0);
    
    // Cria as tasks
    xTaskCreate(Task1, "ButtonTask", 128, NULL, 2, NULL);
    xTaskCreate(Task2, "LedTask", 128, NULL, 1, NULL);
    
    // Inicia o escalonador
    vTaskStartScheduler();
    
    // Nunca deveria chegar aqui
    printf("Erro no escalonador!\n");
    while(1);
}