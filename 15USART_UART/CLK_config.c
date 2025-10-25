

//author: Julian Adrian Viana Palomo
#include "stm32f10x.h"                  // Device header
#include "address.h"	//direcciones de memoria
#include "stdint.h" //C


void CLKconfig(void) {
    
    RCC_CR |= (1<<0);// HSI ON
    while (!(RCC_CR & (1<<1)));  // espera a que HSI esté listo

    // Selecciona HSI como SYSCLK
    RCC_CFGR &= ~(0x3); //limpia registro
    while ((RCC_CFGR & 0xC) != 0x0);  // espera SYSCLK = HSI
	
	
	//habilito perifericos
    RCC_APB2ENR |= (1 << 2);   // IOPAEN
    RCC_APB2ENR |= (1 << 3);   // IOPBEN
    RCC_APB2ENR |= (1 << 14);  // USART1EN
}
