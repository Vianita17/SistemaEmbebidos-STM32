


//Registro de Direcciones 

#define RCC_APB2ENR (*(volatile uint32_t*)0x40021018) //habilitar perifericos GPIOa, GPIOB, TIM1

// GPIOx registros
#define GPIOA_CRH   (*(volatile uint32_t*)0x40010804)  // configuraci?n de pines PA8..PA15
#define GPIOA_ODR   (*(volatile uint32_t*)0x4001080C)  // salida de datos (ODR)
#define GPIOA_IDR   (*(volatile uint32_t*)0x40010808)  // entrada de datos (IDR)

// GPIOB registros
#define GPIOB_CRL   (*(volatile uint32_t*)0x40010C00)  // configuraci?n de pines PB0..PB7
#define GPIOB_BSRR  (*(volatile uint32_t*)0x40010C10)  // registro de set/reset r?pido

// TIM1 registros 
#define TIM1_CR1    (*(volatile uint32_t*)0x40012C00)  // control principal (CEN, DIR, etc.)
#define TIM1_SMCR   (*(volatile uint32_t*)0x40012C08)  // configuraci?n de esclavo (modo externo)
#define TIM1_CCMR1  (*(volatile uint32_t*)0x40012C18)  // configuraci?n canales (entrada/salida)
#define TIM1_CCER   (*(volatile uint32_t*)0x40012C20)  // habilita comparadores/captura
#define TIM1_CNT    (*(volatile uint32_t*)0x40012C24)  // contador actual
#define TIM1_PSC    (*(volatile uint32_t*)0x40012C28)  // prescaler
#define TIM1_ARR    (*(volatile uint32_t*)0x40012C2C)  // valor de auto-reload (m?ximo)
#define TIM1_EGR    (*(volatile uint32_t*)0x40012C14)  // event generation (para forzar update)
