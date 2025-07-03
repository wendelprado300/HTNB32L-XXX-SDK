#include "HT_GPIO_Demo.h"

// Função de delay simples
void Delay_ms(uint32_t milliseconds) {
    for(uint32_t i = 0; i < milliseconds * 10000; i++) {
        __NOP(); // Pode ser removido se não disponível
    }
}

void HT_GPIO_InitLed(void) {
    GPIO_InitType GPIO_InitStruct = {0};
    GPIO_InitStruct.af = PAD_MuxAlt0;
    GPIO_InitStruct.pad_id = LED_PAD_ID;
    GPIO_InitStruct.gpio_pin = LED_GPIO_PIN;
    GPIO_InitStruct.pin_direction = GPIO_DirectionOutput;
    GPIO_InitStruct.init_output = 1; // Inicia com LED apagado
    GPIO_InitStruct.pull = PAD_AutoPull;
    GPIO_InitStruct.instance = LED_INSTANCE;
    GPIO_InitStruct.exti = GPIO_EXTI_DISABLED;
    HT_GPIO_Init(&GPIO_InitStruct);
}

void HT_GPIO_App(void) {
    ht_printf("Blink LED Example (No HT_Timing.h)\n");
    HT_GPIO_InitLed();
    slpManNormalIOVoltSet(IOVOLT_3_30V);

    while(1) {
        HT_GPIO_WritePin(LED_GPIO_PIN, LED_INSTANCE, LED_ON);
        ht_printf("LED ON\n");
        Delay_ms(1000); // 1 segundo
        
        HT_GPIO_WritePin(LED_GPIO_PIN, LED_INSTANCE, LED_OFF);
        ht_printf("LED OFF\n");
        Delay_ms(1000); // 1 segundo
    }
}