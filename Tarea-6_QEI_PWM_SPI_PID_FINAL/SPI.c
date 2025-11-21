/*
MAX7219	|		TIVA		|
CLK			|		PA2			|
LOAD/CS	|		PA3			|
DIN			|		PA4			|
*/
#include "SPI.h"

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
void MAX7219_Transmite(uint8_t Reg, uint8_t Data){
	GPIO_PORTA_DATA_R &= ~(1 << 3);	// CS = LOW
  while((SSI0_SR_R & SSI_SR_TNF) == 0);  
	
  SSI0_DR_R = ((uint16_t)Reg << 8) | Data; // Acomodamos Data y Reg en 16 bits
  while(SSI0_SR_R & SSI_SR_BSY);  				 // Realizamos transmision.
	
  GPIO_PORTA_DATA_R |= (1 << 3);  // CS = HIGH
}
void MAX7219_Init(void){
	uint8_t i;
	SPI_Init();
	
	// Configuracion MAX7219
  MAX7219_Transmite(MAX7219_REG_SHUTDOWN, 	 0x01); // Salir de shutdown
  MAX7219_Transmite(MAX7219_REG_DECODEMODE,  0xFF); // Decode Mode en 8 dígitos
  MAX7219_Transmite(MAX7219_REG_INTENSITY, 	 0x0F); // Intensidad máxima
  MAX7219_Transmite(MAX7219_REG_SCANLIMIT, 	 0x07); // Scan Limit 8 dígitos
  MAX7219_Transmite(MAX7219_REG_DISPLAYTEST, 0x00); // Display Test OFF
	
	for (i = MAX7219_REG_DIGIT0; i <= MAX7219_REG_DIGIT7; i++) {
		MAX7219_Transmite(i, 0x00);
  }
}
void MAX7219_Transmite_Number (uint32_t Number)
{
	uint8_t i = 1;       // Comenzamos en el Dígito 0 (0x01)
  uint8_t Digit;       // Variable temporal para cada dígito
	if (Number == 0) {
		MAX7219_Transmite(MAX7219_REG_DIGIT0, 0);
    // Opcional: Limpiar el resto de dígitos si es necesario
    return;
	}
	while (Number > 0 && i <= 8) {
		Digit = Number % 10;            // Obtener el último dígito
    MAX7219_Transmite(i, Digit); // Enviarlo al MAX7219
    Number = Number / 10;              // Eliminar el último dígito
    i++;                         // Moverse a la siguiente posición en el display
	}
}