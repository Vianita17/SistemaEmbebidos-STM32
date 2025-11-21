
#ifndef PID_H
#define PID_H

#include <stdint.h> 
#include "Timer.h"

extern volatile uint32_t PID_Flag;

typedef struct {
	float Kp; // Ganacia Proporcional
  float Ki; // Ganancia Integral
  float Kd; // Ganancia Diferencial
	
	float LimMax;	// Limite superior
	float LimMin;	// Limite inferior

  float SetPoint;   // QEI donde el helicoptero es estable

  float Integral;     // Acumulador del error
  float PrevError;    // El error del ciclo anterior (Para calcular la D)

}	PID_Config;

void PID_Init(PID_Config *PID, float Kp, float Ki, float Kd, float SetPoint, float LimMax, float LimMin);
float PID_Compute(PID_Config *PID, float Current);

// Interrupcion 
void Timer0A_Handler(void);

#endif