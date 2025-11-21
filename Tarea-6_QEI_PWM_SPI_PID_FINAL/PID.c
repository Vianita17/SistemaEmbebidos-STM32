#include "PID.h"
#include "tm4c1294ncpdt.h"


volatile uint32_t PID_Flag = 0; // variable global para el ritmo del pid

//Para ejecutar la funcion cada 10 ms
void Timer0A_Handler(void){
	// limpiar la bandera de interrupcion o podria ocasionar problemas
	TIMER0_ICR_R = TIMER_ICR_TATOCINT;
	
	// levanta la bandera para el main
	PID_Flag = 1;
}

void PID_Init(PID_Config *PID, float Kp, float Ki, float Kd, float SetPoint, float LimMax, float LimMin)
{
	// Valor inicial PID
	PID->Kp = Kp;
	PID->Ki = Ki;
	PID->Kd = Kd;
	PID->SetPoint = SetPoint;
	
	// Limites para Anti-Windup
	PID->LimMax = LimMax;
	PID->LimMin = LimMin;
	
	// Inicializacion de la memoria.
	PID->Integral = 0.0f;
	PID->PrevError = 0.0f;
	
}
float PID_Compute(PID_Config *PID, float Current)
{
	// Calculo del Error
	float Error = PID->SetPoint - Current;
	
	// Accion PID.
	
	// Accion P
	float P_Term = PID->Kp * Error;
	// Accion I.
	PID->Integral += Error;
	float I_Term = PID->Ki * PID->Integral;
	
	// Limite Anti-Windup. Esto evita que el error crezca 
	// 	infinitamente si el motor se traba.
	if(I_Term > PID->LimMax){
		I_Term = PID->LimMax;
		PID->Integral = PID->LimMax / PID->Ki;
	} else if (I_Term < PID->LimMin){
		I_Term = PID->LimMin;
		PID->Integral = PID->LimMin / PID->Ki;
	}
	
	// Accion D.
	float D_Term = PID->Kd * (Error - PID->PrevError);
	PID->PrevError = Error;
	
	// Salida
	float Output = P_Term + I_Term + D_Term;
	
	// Saturacion de salida
	if (Output > PID->LimMax){
		Output = PID->LimMax;
	} else if(Output < PID->LimMin) {
		Output = PID->LimMin;
	}
	
	return Output;
}
