
//---TIM_config
#include "stm32f10x.h"  // librer�a de soporte de ST (no se usa directo aqu�)

/* --- Registros (direcciones absolutas) --- */
// relojes perif�ricos (APB2ENR controla habilitaci�n de GPIO y TIM1)
#define RCC_APB2ENR (*(volatile uint32_t*)0x40021018)


#define GPIOA_IDR   (*(volatile uint32_t*)0x40010808)  // entrada de datos (IDR)

// GPIOB registros
#define GPIOB_CRL   (*(volatile uint32_t*)0x40010C00)  // configuraci�n de pines PB0..PB7
#define GPIOA_CRH   (*(volatile uint32_t*)0x40010804)  // configuraci�n de pines PA8..PA15
#define GPIOB_BSRR  (*(volatile uint32_t*)0x40010C10)  // registro de set/reset r�pido
#define GPIOA_ODR   (*(volatile uint32_t*)0x4001080C)  // salida de datos (ODR)

// TIM1 registros 
#define TIM1_CR1    (*(volatile uint32_t*)0x40012C00)  // control principal (CEN, DIR, etc.)
#define TIM1_SMCR   (*(volatile uint32_t*)0x40012C08)  // configuraci�n de esclavo (modo externo)
#define TIM1_CCMR1  (*(volatile uint32_t*)0x40012C18)  // configuraci�n canales (entrada/salida)
#define TIM1_CCER   (*(volatile uint32_t*)0x40012C20)  // habilita comparadores/captura
#define TIM1_CNT    (*(volatile uint32_t*)0x40012C24)  // contador actual
#define TIM1_PSC    (*(volatile uint32_t*)0x40012C28)  // prescaler
#define TIM1_ARR    (*(volatile uint32_t*)0x40012C2C)  // valor de auto-reload (m�ximo)
#define TIM1_EGR    (*(volatile uint32_t*)0x40012C14)  // event generation (para forzar update)

void config_TIM(void);

void config_TIM(void){
    // habilitar relojes de GPIOA y TIM1 
    RCC_APB2ENR |= (1 << 2);   // GPIOA
    RCC_APB2ENR |= (1 << 11);  // TIM1

    // configurar TIM1 para contador interno 
    TIM1_CR1  = 0;        // contador apagado
    TIM1_PSC  = 0xFFFF;     // prescaler -> ajusta seg�n tu reloj APB2 (ej: 72MHz/65535 = 122 Hz)
    TIM1_ARR  = 0xFFFF;     // auto-reload (m�ximo valor 65535)
    TIM1_EGR  = 1;        // generar actualizaci�n inmediata

    // inicializar contador 
    TIM1_CNT = 0;

    // habilitar TIM1 
    TIM1_CR1 |= 1;        // CEN=1 -> iniciar contador
}


