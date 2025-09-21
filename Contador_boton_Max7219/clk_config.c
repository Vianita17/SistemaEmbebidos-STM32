
//---clk_config
#include "stm32f10x.h"
 
#define RCC_CR     (*(volatile uint32_t*) 0x40021000) //registro de encendido de osciladores
#define RCC_CFGR   (*(volatile uint32_t*) 0x40021004) //registro seleccion de  osciladores

void conf_clk(void) {
    
    RCC_CR |= (1<<0);// HSI ON
    while (!(RCC_CR & (1<<1)));  // espera a que HSI esté listo

    // Selecciona HSI como SYSCLK
    RCC_CFGR &= ~(0x3); //limpia registro
    while ((RCC_CFGR & 0xC) != 0x0);  // espera SYSCLK = HSI
}
