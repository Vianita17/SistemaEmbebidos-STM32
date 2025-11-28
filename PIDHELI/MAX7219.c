#include <stdint.h>                          // Tipos enteros estándar
#include "tm4c1294ncpdt.h"                   // Registros y bits de la TM4C1294
#include "MAX7219.h"                         // Prototipos del módulo MAX7219
void SPI_Init(void)
{
	// Configuracion Puerto A
  SYSCTL_RCGCGPIO_R |= (1 << 0);    // GPIO A
  SYSCTL_RCGCSSI_R  |= (1 << 0);    // SSI0
	
  while((SYSCTL_PRGPIO_R & (1 << 0)) == 0);
  while((SYSCTL_PRSSI_R &  (1 << 0)) == 0);
	
	// PA2 = CLK y PA4 = DIN
  GPIO_PORTA_AFSEL_R |= (1 << 2) | (1 << 4); // Activamos Funcion Alternativa
  GPIO_PORTA_DEN_R   |= (1 << 2) | (1 << 4); // Activamos entrada digital
	GPIO_PORTA_PCTL_R   = (GPIO_PORTA_PCTL_R & 0xFF0000FF) | 0x000F0F00;
  
  // PA3 como GPIO (CS manual)
  GPIO_PORTA_AFSEL_R &= ~(1 << 3);	// Apagamos funcion alternativa.
  GPIO_PORTA_DEN_R   |=  (1 << 3);	// Activamos entrada digital.
  GPIO_PORTA_DIR_R   |=  (1 << 3);	// Activamos como salida.
	GPIO_PORTA_DATA_R  |= (1 << 3);   // CS HIGH por default.
	
	GPIO_PORTA_AMSEL_R  = 0;		 // Desactivamos ADC para todos los pines
	
	// Configuracion SPI
	SSI0_CR1_R = 0;               // Limpiamos registro
																// Warming: No es necesario hacer otra configuracion por ahora
																//						ya que, en este estado es modo maestro y no esta
																//						habilitado, la habilitacion es al final.
																
	SSI0_CR0_R = 0;										// Limpiamos registro.
	SSI0_CR0_R |= SSI_CR0_FRF_MOTO;   // Freescale SPI (0x0)
  SSI0_CR0_R |= SSI_CR0_DSS_16;     // 16 bits
	
  SSI0_CPSR_R = 2;              // SPI clk = 8 MHz
  SSI0_CC_R  = 0;               // Reloj sistema

  SSI0_CR1_R |= (1 << 1);       // Activar SPI
}
void Transmite(uint8_t Reg, uint8_t Data){
	GPIO_PORTA_DATA_R &= ~(1 << 3);
  while((SSI0_SR_R & SSI_SR_TNF) == 0);  
	
  SSI0_DR_R = ((uint16_t)Reg << 8) | Data;
  while(SSI0_SR_R & SSI_SR_BSY); 
	
  GPIO_PORTA_DATA_R |= (1 << 3);
}
void MAX7219_Init(void){
	SPI_Init();
	
	// Configuracion MAX7219
  Transmite(MAX7219_REG_SHUTDOWN, 	 0x01);
  Transmite(MAX7219_REG_DECODEMODE,  0xFF);
  Transmite(MAX7219_REG_INTENSITY, 	 0x0F);
  Transmite(MAX7219_REG_SCANLIMIT, 	 0x07);
  Transmite(MAX7219_REG_DISPLAYTEST, 0x00); 
	
	ZERO();
}
void Transmite_Number (uint32_t Number)
{
		int residuo;
		int NumDigito = 1;
    do{
        residuo = Number % 10;
        Transmite(NumDigito, residuo); 
        Number /= 10;
        NumDigito++;
    }while(Number > 0);
}
void ZERO(void){
	for (int k=0; k<8; k++){
		Transmite(k, 0x00); 
	}
}