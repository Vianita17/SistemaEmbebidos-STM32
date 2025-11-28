// PWM.h
// Driver de PWM0 en PF0 (M0PWM0) y PF1 (M0PWM1)
// TM4C1294NCPDT

#ifndef PWM_H
#define PWM_H

#include <stdint.h>

// Inicializa PWM0A en PF0 con periodo y duty dados
void PWM0A_Init(uint16_t period, uint16_t duty);
// Cambia solo el duty de PWM0A (PF0)
void PWM0A_Duty(uint16_t duty);

// (Opcional) Inicializa PWM0B en PF1 con mismo periodo y duty
void PWM0B_Init(uint16_t period, uint16_t duty);
// Cambia solo el duty de PWM0B (PF1)
void PWM0B_Duty(uint16_t duty);

#endif
