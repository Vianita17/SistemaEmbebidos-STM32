#ifndef GPIOS_H
#define GPIOS_H

#include "tm4c1294ncpdt.h"      // Definiciones del microcontrolador Tiva C
#include <stdint.h> 

// ----- PUERTO F
#define LED3 (1<<0) //PF4
#define LED4 (1<<1) //PF0

// ----- PUERTO N
#define LED1 (1<<1)	//PN0
#define LED2 (1<<0) //PN1

// ----- PUERTO L -----
#define BTN_SET_MASK  (1 << 0) // PL0
#define BTN_UP_MASK   (1 << 1) // PL1
#define BTN_DOWN_MASK (1 << 2) // PL2
#define BTN_FORMAT    (1 << 3) // PL3
#define BTN_CALENDAR  (1 << 4) // PL4

void Button_config(void); //FUNCIOND DE CONFIGURACION BOTONES
void LEDS_config(void); //FUNCION DE CONFIG LED


#endif