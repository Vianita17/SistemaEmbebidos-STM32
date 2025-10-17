
//author: Julian Viana Palomo

#include "stm32f10x.h"                  // Device header

//primer aso declarar las direcciones de los registros

//APB2ENR -> habilitar perifericos
#define RCC_APB2ENR   	(*(volatile uint32_t*) 0x40021018)

//CR H/L -> configuracion alta y baja de los piens
#define GPIOA_CRL   	(*(volatile uint32_t*) 0x40010800) //PORTA CRL -> boton A0
#define GPIOC_CRH   	(*(volatile uint32_t*) 0x40011004) //PORTC CRH -> LED PC13 el integrado

//AFIO_CR1 -> control del exti0
#define AFIO_EXTICR1 	(*(volatile uint32_t*) 0x40010008) 

//EXTI_IMR -> configuracion de la mascara de interrupciones
#define EXTI0_IMR			(*(volatile uint32_t*) 0x40010400)
	
//EXTI_RTSR -> configuracion de la mascara de rising trigger
#define EXTI0_RTSR		(*(volatile uint32_t*) 0x40010408)

//EXTI_PR -> registro de detecccion de eventos(triggers)
#define EXTI0_PR		(*(volatile uint32_t*) 0x40010414)
	
//NVICISER -> habilitar NVIC IRQ
#define NVIC_ISER		(*(volatile uint32_t*) 0xE000E100)
	
//ODR -> prender o apagar led
#define GPIOA_ODR   (*(volatile uint32_t*) 0x4001080C)
#define GPIOC_ODR   (*(volatile uint32_t*) 0x4001100C)
	
//BSRR -> prender o apagar led
#define GPIOA_BSRR   (*(volatile uint32_t*) 0x40010810)
#define GPIOC_BSRR	 (*(volatile uint32_t*) 0x40011010)
//IDR -> leer estado de los pines
#define GPIOA_IDR   (*(volatile uint32_t*) 0x40010808)
#define GPIOC_IDR	 (*(volatile uint32_t*) 0x40011008)
	

void Interrupcion(void){ 
	if (EXTI0_PR & (1<<0)){    //verifico que la bandera del trigger este activa 
														 // 0:no trgger --- 1:trigger request
				GPIOC_BSRR = (1 << 29);       // Enciende LED (PC13 bajo)
        for (volatile int i = 0; i < 1000000; i++); // Pequeño retardo visible
        GPIOC_BSRR = (1 << 13);         // Apaga LED (PC13 alto)
        EXTI0_PR = (1 << 0);             // Limpia bandera
    }
		
}
//static int last_state = 0; //funcion  para mejor la lectura del boton pero no funciono
/*void Interrupcion(void){

	if (EXTI0_PR & (1<<0)){    
				int current_state = (GPIOA_IDR & (1 << 0)) ? 1 : 0; //definir estado actual
        if (current_state == 1 && last_state == 0){
            for (volatile uint32_t d=0; d<50000; d++) { } //debounce
            // Verificar que el botón sigue presionado después del debounce
            if (GPIOA_IDR & (1 << 0)) {
                GPIOC_BSRR = (1 << 29);       // Enciende LED (PC13 bajo)
            }else{
							GPIOC_BSRR = (1<<13);
						}
        }else{
					GPIOC_BSRR = (1<<13);
					
				}
				//GPIOC_BSRR = (1<<13);
        last_state = current_state;// cambiamos de estado
        EXTI0_PR = (1 << 0);             // Limpia bandera trigger
    }
}*/
	
int main (void){
	//paso dos activar PORTA,PORTC y AFIO
	RCC_APB2ENR |= (1 << 0); //->AFIO en
	RCC_APB2ENR |= (1 << 2); //PORTA en
	RCC_APB2ENR |= (1 << 4); //PORTC en
	
	//paso dos configurar A0 y PC13
	
	GPIOA_CRL &= ~(0xF << 0); //limpio pin0
	GPIOA_CRL |= (0x8 << 0); // A0 -> input dowm/pull
	GPIOA_ODR &= ~(1 << 0);     // Pull-down
	
	GPIOC_CRH &= ~(0xF << 20); //limpio pin13
	GPIOC_CRH |= (0x3 << 20); //C13 -> output push/pull 50Mhz
	GPIOC_BSRR = (1<<13);//led apagado
	
	AFIO_EXTICR1 &= ~(0xFF << 0); //limpio EXTI0
	AFIO_EXTICR1 |= (0x00 << 0); // EXTI0 -> PORTA
	
	NVIC_ISER |=  (1 << 6); //habbilitar funcion IRQ correspondiente al EXTI0
	
	EXTI0_IMR |= (1 << 0); //por el puerto A0  1:not mask 0:mask
	EXTI0_RTSR |= (1 << 0); //detectar la subida en el puerto A0 1:trigger 0:not trigger
	//cuando detecte el franco de subida  va llamar a la funcion de NVIC IRQ coorrespondiente al EXTI0
	//en nuestro caso es : EXTI0_IRQHandler
	
	
	/*  it stops executing instructions and enters
		sleep mode*/
	
	__WFI(); //funcion de ensamblador que causa el modo spleep de forma automatica de STM32
	//sin enmbargo para fines practicos lo usamos para asegurar que la srm32 este en modo reposo
	//y cuando llegue una interrupcion nosostros con nuestra configuracion deberia realizar
	//lo que tenemos en nuestra funcion de interrupcion.
 	
	while(1){}

}
