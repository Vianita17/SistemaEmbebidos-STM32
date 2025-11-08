
#include <stdint.h>   // libreria para tipos enteros (uint32_t, etc.)
#include "stm32f10x.h"  // libreria de soporte de ST
#include "adress.h"

//funcion para configurar pin
void configPorts(){
		
	//habilito GPIO A, B y TIM1
    RCC_APB2ENR |= (1 << 2);   //habilito reloj a GPIOA  hex -> 0x00000004
    RCC_APB2ENR |= (1 << 3);   //habilito reloj GPIO B hex -> 0x00000008
	//si lo haria en una sola linea seria 0x0000000C
		
	//configuro PA8 como input down/pull
		GPIOA_CRH &= ~(0xF << 0);   //limpio cualquier configuracion en el pin
    GPIOA_CRH |=  (0x8 << 0);   //escribimos valor para input down/pull
																// CNF [1:0] = 10 and MODE[1:0] = 00
    GPIOA_ODR |=  (1 << 8);     // escribimos 1 en el puerto, para conectar nuestro boton a GND
																//es decir esta pull up el pin
	//configuro PB0 y PB1
		GPIOB_CRL &= ~((0xF << 0) | (0xF << 4));  //limpiamos configuracion en pin 0 y 1
    GPIOB_CRL |=  ((0x2 << 0) | (0x2 << 4)); // configuramos ambos pines como push/pull 2 Mhz
																							// CNF [1:0] = 00 and MODE[1:0] = 10
}

//funcion para configurar TIM
void configTIM(){
	
		RCC_APB2ENR |= (1 << 11);  //habilito reloj a TIM1 hex -> 0x00000800*/
		RCC_APB2ENR |= (1 << 11); //habilitamos funcion alternativa
	
		//config TIM1 para contar pulsos externos en PA8 
    TIM1_CR1  = 0;             //contador apagado, direcci�n ascendente
    TIM1_PSC  = 0;             // prescaler = 0 -> cada pulso = +1 al contador 
    TIM1_ARR  = 3;             // auto-reload = 3 -> contador cuenta 0..3 (mod 4) 
    TIM1_EGR  = (1 << 0);      // UG: forzar actualizaci�n de PSC y ARR */

    // cconfig canal 1 como entrada TI1 con filtro 
    TIM1_CCMR1 = (0x1 << 0) | (0xF << 4);  // CC1S=01 -> mapea TI1, IC1F=1111 -> filtro max

    // solo flanco de bajada
    TIM1_CCER  = (1 << 0) | (1 << 1);  // CC1E=1 -> habilita captura en CH1, CC1P=1 -> flanco descendente 

   
		
			/*
			Normalmente el modo maestro esta por defecto
			TIM1 funciona con su propio reloj interno (derivado de APB2).
			Si se deja asi, el contador CNT avanzar�a en cada tick de reloj, a una frecuencia muy alta (MHz).
			Entonces al no querer eso nosotros el TIM1 no se controla a si mismo, sino que depende de un trigger externo
			Por eso:
			TS=101  fuente de trigger = TI1FP1 (entrada filtrada del canal 1, que es PA8).
			SMS=111  modo esclavo "External Clock Mode 1" (el trigger se convierte en el reloj del contador).
			*/

		// modo esclavo: External Clock Mode 1
    // -> TRGI=TI1FP1 (TS=101), SMS=111 (contador usa TRGI como clock)
    TIM1_SMCR  = (0b101 << 4) | (0b111 << 0);
    
		
		// inicia el contador en 0
    TIM1_CNT = 0;
    TIM1_CR1 |= (1 << 0);      // CEN=1 -> habilita contador
}


//funcion para encender los leds
void encender(uint32_t bit){
	
	
    // si bit0 en '1' -> enciende PB0, si no -> apaga PB0
    if (bit & 0x1) { //comparo si el bit0 esta en uno
					GPIOB_BSRR = (1 << 0); //si esta enciendo pb0
		} else { 
					GPIOB_BSRR = (1 << 16); //sino apago pb0
		}
    // si bit1 est� en '1' -> enciende PB1, si no -> apaga PB1
    if (bit & 0x2) {  //comparo si el bit1 esta en uno
					GPIOB_BSRR = (1 << 1); //si esta enciendo pb0
		} else { 
					GPIOB_BSRR = (1 << 17); //sino apago pb0
		}//esto lo hago para tener control de los leds cuando se encienden y apagan
}


int main(void){
    
    configPorts(); //enciendo y configuro pines
		configTIM();	//configuro TIM
    encender(0);  // apago ambos leds inicialmente


    while (1){
        uint32_t n = TIM1_CNT & 0x3;  /* se queda con los bits [1:0] */
        encender(n);                  /* los muestra en PB1:PB0 */
    }
}
