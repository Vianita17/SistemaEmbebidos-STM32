
//---TIM_config
#include "stm32f10x.h"  // librería de soporte de ST (no se usa directo aquí)

/* --- Registros (direcciones absolutas) --- */
// relojes periféricos (APB2ENR controla habilitación de GPIO y TIM1)
#define RCC_APB2ENR (*(volatile uint32_t*)0x40021018)


#define GPIOA_IDR   (*(volatile uint32_t*)0x40010808)  // entrada de datos (IDR)

// GPIOB registros
#define GPIOB_CRL   (*(volatile uint32_t*)0x40010C00)  // configuración de pines PB0..PB7
#define GPIOA_CRH   (*(volatile uint32_t*)0x40010804)  // configuración de pines PA8..PA15
#define GPIOB_BSRR  (*(volatile uint32_t*)0x40010C10)  // registro de set/reset rápido
#define GPIOA_ODR   (*(volatile uint32_t*)0x4001080C)  // salida de datos (ODR)

// TIM1 registros 
#define TIM1_CR1    (*(volatile uint32_t*)0x40012C00)  // control principal (CEN, DIR, etc.)
#define TIM1_SMCR   (*(volatile uint32_t*)0x40012C08)  // configuración de esclavo (modo externo)
#define TIM1_CCMR1  (*(volatile uint32_t*)0x40012C18)  // configuración canales (entrada/salida)
#define TIM1_CCER   (*(volatile uint32_t*)0x40012C20)  // habilita comparadores/captura
#define TIM1_CNT    (*(volatile uint32_t*)0x40012C24)  // contador actual
#define TIM1_PSC    (*(volatile uint32_t*)0x40012C28)  // prescaler
#define TIM1_ARR    (*(volatile uint32_t*)0x40012C2C)  // valor de auto-reload (máximo)
#define TIM1_EGR    (*(volatile uint32_t*)0x40012C14)  // event generation (para forzar update)

void config_TIM(void);

void config_TIM(void) {
    // Habilitar relojes de GPIOA y TIM1 */
    RCC_APB2ENR |= (1 << 2);     // habilita reloj GPIOA 
    RCC_APB2ENR |= (1 << 11);    // habilita reloj de TIM1

    /* 2) Configurar PA9 como entrada con PULL-UP */
    GPIOA_CRH &= ~(0xF << 4);    /* limpia configuración de PA9 (bits [7:4]) */
    GPIOA_CRH |= (0x8 << 4);     /* modo entrada con pull-up/pull-down */
    GPIOA_ODR |= (0 << 9);       /* ODR9=0 -> selecciona PULL-down */

    // Configuración del TIM1 para contar pulsos externos en PA9 */
    TIM1_CR1 = 0;                // contador apagado, dirección ascendente
    TIM1_PSC = 0;                // prescaler = 0 -> cada pulso = +1 al contador
    TIM1_ARR = 0xFFFF;					//valor maximo del ARR o cuentas
    TIM1_EGR = (1 << 0);         // UG: forzar actualización de PSC y ARR

    //Configuración canal 2 como entrada TI2 con filtro 
    TIM1_CCMR1 &= ~(0xFF << 8);  // limpia bits de CC2 (CC2S y IC2F)
    TIM1_CCMR1 |= (0x1 << 8) | (0xF << 12); // CC2S=01 -> mapea TI2, IC2F=1111 -> filtro máximo 

    /* Solo flanco de bajada */
    TIM1_CCER &= ~(0xF << 4); /* limpia bits de CC2 (CC2E y CC2P) */
    TIM1_CCER |= (1 << 4) | (1 << 5); // CC2E=1 -> habilita captura en CH2, CC2P=1 -> flanco descendente

    /* Modo esclavo: "External Clock Mode 1"
     -> TRGI=TI2FP2 (TS=110), SMS=111 (contador usa TRGI como clock)*/
    TIM1_SMCR = (0b110 << 4) | (0b111 << 0);

    /* Inicia el contador en 0 */
    TIM1_CNT = 0;								//contador en ceros
    TIM1_CR1 |= (1 << 0);        /* CEN=1 -> habilita contador */
}

/*void config_TIM(void){
    // 1) Habilitar relojes de GPIOA y TIM1 
    RCC_APB2ENR |= (1 << 2);   // GPIOA
    RCC_APB2ENR |= (1 << 11);  // TIM1

    // 2) Configurar TIM1 para contador interno 
    TIM1_CR1  = 0;        // contador apagado
    TIM1_PSC  = 7199;     // prescaler -> ajusta según tu reloj APB2 (ej: 72MHz/7200 = 10kHz)
    TIM1_ARR  = 0xFFFF;     // auto-reload (máximo valor 0..9999)
    TIM1_EGR  = 1;        // generar actualización inmediata

    // 3) Inicializar contador 
    TIM1_CNT = 0;

    // 4) Habilitar TIM1 
    TIM1_CR1 |= 1;        // CEN=1 -> iniciar contador
}*/


