/*
Motor		|		TIVA		|
CLK			|		PA2			|
LOAD/CS	|		PA3			|
DIN			|		PA4			|
*/

#include "PWM.h"

void PWMA_M0_Init (uint16_t Period_Data, uint16_t Duty_Data)
{
	SYSCTL_RCGCPWM_R |= (1 << 0);		// Habilitamos el reloj PWM 0
	SYSCTL_RCGCGPIO_R |= (1 << 5);  // Habilitamos el puerto F.
	
	// Esperamos la estabilizacion de los relojes.
	while((SYSCTL_PRPWM_R  & (1 << 0)) == 0);
	while((SYSCTL_PRGPIO_R & (1 << 5)) == 0);
	
	// Configuramos los Pines a usar. PF0
	GPIO_PORTF_AFSEL_R |=  (1 << 0); // Activamos funcion altenariva en PF0
	GPIO_PORTF_DEN_R   |=  (1 << 0); // Activamos funcion digital en PF0
	GPIO_PORTF_AMSEL_R &= ~(1 << 0); // Aseguramos la desactivacion de la funcion analogica en PF0
	
	// Asignamos el valor 6 al PCTL.
	GPIO_PORTF_PCTL_R &= ~(0xF); // Limpiamos los primeros 4 bits
	GPIO_PORTF_PCTL_R |=  (0x6); // Asignamos el valor 6. Vease pagina 1672
	
	// Configuramos el reloj del PWM. 
	// [Esto cambia al transistor MOSFET que se ponga al proyecto.]
	// PWM0_CC_R &= ~(1 << 8); PWM SIN DIVISOR., por default es asi.
	PWM0_CC_R |=  PWM_CC_USEPWM;		// Usamos el divisor PWM.
	PWM0_CC_R &= ~PWM_CC_PWMDIV_M;	// Limpiamos el divisor
	PWM0_CC_R |=  PWM_CC_PWMDIV_64;	// Configuramos el divisor a 64. 
	
	// Inicializamos el generador PWM0
	PWM0_0_CTL_R = 0x0; // Desactivamos todos los generadores de PWM.
	
	// Realizamos accion de carga (2) y Configuramos para Count-Down (6)
	PWM0_0_GENA_R = ((0b11 << 2) | (0b10 << 6));
	PWM0_0_LOAD_R = Period_Data - 1;
	PWM0_0_CMPA_R = Duty_Data - 1;
	
	PWM0_0_CTL_R  |= 0x1; // Encendemos el generador PWM 0.
	PWM0_ENABLE_R |= 0x1; // Habilitamos salida PWM 0
}

/*void PWMA_M0_Set_Duty (uint16_t Duty_Data)
{
	PWM0_0_CMPA_R = Duty_Data - 1;
}*/

void PWMB_M0_Init (uint16_t Period_Data, uint16_t Duty_Data)
{
	// Habilitamos el reloj del modulo PWM.
	SYSCTL_RCGCPWM_R |= (1 << 0);
	
	// Habilitamos el puerto F.
	SYSCTL_RCGCGPIO_R |= (1 << 5);
	
	// Esperamos la estabilizacion de los relojes.
	while((SYSCTL_PRPWM_R  & (1 << 0)) == 0);
	while((SYSCTL_PRGPIO_R & (1 << 5)) == 0);
	
	// Configuramos los Pines a usar. Primero activamos las
	//	Funciones Aleternativas.
	GPIO_PORTF_AFSEL_R |=  (1 << 1); // Activamos funcion altenariva en PF0
	GPIO_PORTF_DEN_R   |=  (1 << 1); // Activamos funcion digital en PF0
	GPIO_PORTF_AMSEL_R &= ~(1 << 1); // Aseguramos la desactivacion de la funcion analogica en PF0
	
	// Asignamos el valor 6 al PCTL.
	GPIO_PORTF_PCTL_R &= ~(0xF0); // Limpiamos los primeros 4 bits
	GPIO_PORTF_PCTL_R |=  (0x60); // Asignamos el valor 6. Vease pagina 1672
	
	// Configuramos el reloj del PWM. 
	// [Esto cambia al transistor MOSFET que se ponga al proyecto.]
	// PWM0_CC_R &= ~(1 << 8); PWM SIN DIVISOR., por default es asi.
	PWM0_CC_R |= PWM_CC_USEPWM;			// Usamos el divisor PWM.
	PWM0_CC_R &= ~PWM_CC_PWMDIV_M;	// Limpiamos el divisor
	PWM0_CC_R |= PWM_CC_PWMDIV_64;	// Configuramos el divisor a 64. 
	
	// Inicializamos el generador PWM0
	PWM0_0_CTL_R = 0x0; // Desactivamos todos los generadores de PWM.
	
	// Realizamos accion de carga (2) y Configuramos para Count-Down (6)
	PWM0_0_GENB_R = ((0b10 << 2) | (0b11 << 6));
	PWM0_0_LOAD_R = Period_Data - 1;
	PWM0_0_CMPB_R = Duty_Data - 1;
	
	PWM0_0_CTL_R  |= 0x1; // Encendemos el generador PWM 0.
	PWM0_ENABLE_R |= 0x2; // Habilitamos salida PWM 1
}

void PWMB_M0_Set_Duty (uint16_t Duty_Data)
{
	PWM0_0_CMPB_R = Duty_Data - 1;
}

void PORT_Init(void){
	// Habilitamos el reloj del Puerto J
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R8;			
	//Esperamos la estabilizacion del reloj
  while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R8) == 0);
	
	// Activamos funcion Digital en PJ0
  GPIO_PORTJ_AFSEL_R &= ~0x03;  // Desactivamos funcion alternativa de PJ0
  GPIO_PORTJ_AMSEL_R &= ~0x03;	// Desactivamos funcion analógica de PJ0
  GPIO_PORTJ_DEN_R |= 0x03;     // Activamos función digital de PJ0
  GPIO_PORTJ_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFFF00);	// Limpiamos PCTL
	
	GPIO_PORTJ_PUR_R |= 0x03;									// Activamos resistencia pull up
																						
}

uint16_t PWMTester(uint16_t x){
	if ((GPIO_PORTJ_DATA_R &=0X01) == 0)
	{
		for (int i = 0; i < 2000000000; i++){}
		++x;
	}else{
		if ((GPIO_PORTJ_DATA_R &=0X02) == 0)
		{
			for (int i = 0; i < 2000000000; i++){}
			--x;
		}else{
		for (int i = 0; i < 2000000000; i++){}
		}
	}
	
	return x;
}