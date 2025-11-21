/*
Author: Luis Antonio Uc Enriquez
Date: 	11-10-25

Description:
Tarea_#. Realizar un PWM estatico en el tiempo. 

Prototypes:
PWMx_M0_Init {Param: NONE} 
	Funcion para inicializar los puertos necesarios y configuracion
		incial para realizar el funcionamiento del PWM Modulo 0. Aqui
		no se establece el duty cycle para el funcionamiento del PWM.
		Consultar pines y puertos para cada modulo en la pagina 1672
		del manual "Tiva TM4C1294NCPDT Microcontroller Data sheet"

PWMx_M0_Set_Duty {Param: Duty_Data}.
	Establece el duty cycle en base a un valor de 16 bits., se usan
	16 bits debido a que el contador es de este valor (16 bits). 
	Duty_Data: Valor en binario del PWM. 
*/
#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include "tm4c1294ncpdt.h" 

void PWMA_M0_Init(uint16_t Period_Data, uint16_t Duty_Data);
void PWMA_M0_Set_Duty(uint16_t Duty_Data);

void PWMB_M0_Init(uint16_t Period_Data, uint16_t Duty_Data);
void PWMB_M0_Set_Duty(uint16_t Duty_Data);

void PORT_Init(void);
uint16_t PWMTester(uint16_t x);

#endif