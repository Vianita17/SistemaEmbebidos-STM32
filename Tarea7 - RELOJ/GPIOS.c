
#include "GPIOS.h"
// --- CONFIGURACIÓN DEL PUERTO L ---
void PORT_L_enable(void) {
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R10; // Activar Port L
    while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R10) == 0){};
}
void Button_config(void){
		PORT_L_enable();
		GPIO_PORTL_DIR_R &= ~(BTN_SET_MASK | BTN_UP_MASK | BTN_DOWN_MASK | BTN_FORMAT | BTN_CALENDAR); // Entradas
    GPIO_PORTL_DEN_R |= (BTN_SET_MASK | BTN_UP_MASK | BTN_DOWN_MASK | BTN_FORMAT | BTN_CALENDAR);  // Digital
    GPIO_PORTL_PUR_R |= (BTN_SET_MASK | BTN_UP_MASK | BTN_DOWN_MASK | BTN_FORMAT | BTN_CALENDAR);  // Pull-Up
}

void PORT_N_enable(void) {
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12; // Activar Port N
	while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R12) == 0){};
}
void PORT_F_enable(void) {
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; // Activar Port N
	while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) == 0){};
}

void LEDS_config(void){
    PORT_N_enable(); //ENCIENDO PUERTO n
    PORT_F_enable();	//ENCINEDO PUETO f

		GPIO_PORTN_DIR_R |= (LED1 | LED2); 	//SALIDAS
		GPIO_PORTN_DEN_R |= (LED1 | LED2);	//DIGTALES
	
    GPIO_PORTF_DIR_R |= (LED3 | LED4); 	//SALIDAS
    GPIO_PORTF_DEN_R |= (LED3 | LED4);	//DIGITALES
	
		//ESTABLECEMOS QUE ESTEN APAGADOS
		GPIO_PORTN_DATA_R &= ~LED1;
		GPIO_PORTN_DATA_R &= ~LED2;
		GPIO_PORTN_DATA_R &= ~LED3;
		GPIO_PORTN_DATA_R &= ~LED4;


}