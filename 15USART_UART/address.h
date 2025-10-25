//punteros a las memorias

//CLK 
#define RCC_CR     (*(volatile uint32_t*) 0x40021000) //registro de encendido de osciladores
#define RCC_CFGR   (*(volatile uint32_t*) 0x40021004) //registro seleccion de  osciladores
	
//PERIFERICOS
#define RCC_APB2ENR		(*(volatile uint32_t*)	0x40021018)//registro para habilitar puertos/usart



//GPIOx registros

//puertoA
#define GPIOA_ODR   (*(volatile uint32_t*)0x4001080C)  // escritura de datos de datos (ODR)
#define GPIOA_IDR   (*(volatile uint32_t*)0x40010808)  // lectura de datos (IDR)
#define GPIOA_BSRR  (*(volatile uint32_t*)0x40010810)  // registro de set/reset rápido
	
#define GPIOA_CRL   (*(volatile uint32_t*)0x40010800)  // configuración de pines PA0..PA7
#define GPIOA_CRH   (*(volatile uint32_t*)0x40010804)  // configuración de pines PA8..PA15

//puerto B
#define GPIOB_ODR   (*(volatile uint32_t*)0x40010C0C)  // escritura de datos de datos (ODR)
#define GPIOB_IDR   (*(volatile uint32_t*)0x40010C08)  // lectura de datos (IDR)
#define GPIOB_BSRR  (*(volatile uint32_t*)0x40010C10)  // registro de set/reset rápido

#define GPIOB_CRL   (*(volatile uint32_t*)0x40010C00)  // configuración de pines PB0..PB7
#define GPIOB_CRH   (*(volatile uint32_t*)0x40010C04)  // configuración de pines PB8..PB15
	
//USART
#define USART1_SR			(*(volatile uint32_t*)	0x40013800) //registro para banderas de TXE and RXE
#define USART1_DR			(*(volatile uint32_t*)	0x40013804)	//registro de transmision/recibimiento de datos
#define USART1_BRR		(*(volatile uint32_t*)	0x40013808) //registro para configurar el baud rate
	
#define USART1_CR1		(*(volatile uint32_t*)	0x4001380C) //registro de habilitacion y configuracion M, par/impar ..
#define USART1_CR2		(*(volatile uint32_t*)	0x40013810)	//registro configuracion para bit stop
	