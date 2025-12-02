#include <stdint.h>                          // Tipos enteros estándar
#include "tm4c1294ncpdt.h"                   // Registros y bits de la TM4C1294
#include "MAX.h"                         // Prototipos del módulo MAX7219
void SPI_Init(void)
{
	// Configuracion Puerto A
  SYSCTL_RCGCGPIO_R |= (1 << 0);    // GPIO A
  SYSCTL_RCGCSSI_R  |= (1 << 0);    // SSI0
  while((SYSCTL_PRGPIO_R & (1 << 0)) == 0); //check modulo A
  while((SYSCTL_PRSSI_R &  (1 << 0)) == 0); //check modulo ssi
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
void Transmite(uint8_t Reg, uint8_t Data){ //funcion transmite
	GPIO_PORTA_DATA_R &= ~(1 << 3); //data vacio
  while((SSI0_SR_R & SSI_SR_TNF) == 0);  //esperamos que este vacio la bandera de Tx
  SSI0_DR_R = ((uint16_t)Reg << 8) | Data; //mandamos direccion +  dato
  while(SSI0_SR_R & SSI_SR_BSY);  //esperamos a que se envie
  GPIO_PORTA_DATA_R |= (1 << 3); //decimos que ya se envio
}
void MAX7219_Init(void){
	SPI_Init();
	// Configuracion MAX7219
  Transmite(MAX7219_REG_SHUTDOWN, 	 0x01);
  Transmite(MAX7219_REG_DECODEMODE,  0xFF);
  Transmite(MAX7219_REG_INTENSITY, 	 0x0F);
  Transmite(MAX7219_REG_SCANLIMIT, 	 0x07);
  Transmite(MAX7219_REG_DISPLAYTEST, 0x00); 
	ZERO(); //funcion para poner en zeros todos los digitos de MAX
}
void Transmite_Number(uint32_t Number){
    // Extraer HH, MM, SS del número
    uint8_t horas   = (Number / 10000);        // HH
    uint8_t minutos = (Number / 100) % 100;    // MM
    uint8_t segundos= Number % 100;            // SS
    // Mostrar HH
    Transmite(8, horas / 10);   // decenas de hora
    Transmite(7, horas % 10);   // unidades de hora
    // Separador
    Transmite(6, CHAR_DASH);
    // Mostrar MM
    Transmite(5, minutos / 10); // decenas de minuto
    Transmite(4, minutos % 10); // unidades de minuto
    // Separador
    Transmite(3, CHAR_DASH);
    // Mostrar SS
    Transmite(2, segundos / 10); // decenas de segundo
    Transmite(1, segundos % 10); // unidades de segundo
}

void ZERO(void){
	for (int k=0; k<8; k++){
		Transmite(k, 0x00); 
	}
}