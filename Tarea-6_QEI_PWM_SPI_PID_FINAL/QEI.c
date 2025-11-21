/*
|Pin Name |		TIVA	|
---------------------
|Index		|		PL3		|
|Fase A		|		PL1		|
|Fase B		|		PL2		|
*/
#include "QEI.h"

void QEI_Init(uint32_t MaxPosition){
	// Configuracion Pines para QEI
	SYSCTL_RCGCQEI_R |= SYSCTL_RCGCQEI_R0; 		// Se habilita el modulo QEI
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R10; // Se da señal de reloj al puerto L
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R10)==0){}; // Se espera que el puerto L este prendido
		
	GPIO_PORTL_AFSEL_R |=0X0E; 																			// Los pines 1, 2 y 3 del puerto L son funciones alternativas
	GPIO_PORTL_PCTL_R |=(GPIO_PORTL_PCTL_R&0xFFFF000F) | 0x00006660;	// Se sonfigura pines 1, 2 y 3 como QEI	
	GPIO_PORTL_DEN_R |=0X0E;																				// Pines 1,2 y 3 se declaran digitales.
	//GPIO_PORTL_PUR_R |=0X0E; 																				// Activa Pull-Up en pines 1, 2 y 3.
	while ((SYSCTL_PRQEI_R & SYSCTL_PRQEI_R0) == 0); 									// Se espera que el modulo QEI este listo
		
	// Configuro el QEI
	QEI0_CTL_R &= ~QEI_CTL_ENABLE;
		
	QEI0_CTL_R&= ~QEI_CTL_FILTCNT_M;	// Dos ciclos de reloj
	QEI0_CTL_R |= QEI_CTL_FILTEN;			// Se filtra la señal de entrada
	
	//QEI0_CTL_R |= QEI_CTL_VELDIV_1;		// No se dividen los pulsos antes de aplicarlos a QEICOUNT
	QEI0_CTL_R &= ~QEI_CTL_VELEN;  		// No se capura la velocidad
	
	QEI0_CTL_R &= ~QEI_CTL_RESMODE;		// Se incia cuando se llega al valor máximo
	
	QEI0_CTL_R &= ~QEI_CTL_SIGMODE;  	// The internal PhA input operates as the clock (CLK) signal and the internal PhB input operates as the direction (DIR) signal.
	QEI0_CTL_R |= QEI_CTL_CAPMODE; 		// Como SIGMODE=1 este bit no importa
		
	QEI0_MAXPOS_R = (MaxPosition);					// Posición máxima
	
	QEI0_CTL_R |= QEI_CTL_ENABLE; // Activa el QEI
	
	QEI0_POS_R = 0;
}
uint32_t READ_QEI0Position(void)
{
	return QEI0_POS_R;
}
