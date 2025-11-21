// Reloj TIVA 16 MHz
#include "PID.h"
#include "PWM.h"
#include "QEI.h"
#include "SPI.h"

// Periodo PWM Inicial
#define PWM_Periodo_Init 25000
// Posicion Maxima para el QEI
#define MAX_POSITION_QEI 1000 

// Inicializacion PID {Ajus	tar}
#define KP_GAIN  0.8
#define KI_GAIN  0.01
#define KD_GAIN  0.05

// SetPoint y Limites
#define QEI_TARGET 400		// Valor estable
#define PWM_MIN 8000.0f 		// Valor minimo del PWM para que NO se apague el motor. 32%
#define PWM_MAX 24750.0f		// Valor Macimo del PWM. 99%


PID_Config Helicoptero;


void PWMA_M0_Set_Duty(uint16_t duty)
{
    if(duty > PWM0_0_LOAD_R)
        duty = PWM0_0_LOAD_R;

    if(duty < PWM_MIN)
        duty = PWM_MIN;

    PWM0_0_CMPA_R = PWM0_0_LOAD_R - duty;
}

int main(void)
{
	MAX7219_Init();							// Inicializa comunicacion SPI con MAX7219
	QEI_Init(MAX_POSITION_QEI);	// Inicializa el modulo QEI

	PWMA_M0_Init(PWM_Periodo_Init, PWM_MIN);	// Inicializacion del PWM
	
	// Inicializamos PID
	PID_Init(&Helicoptero, KP_GAIN, KI_GAIN, KD_GAIN, QEI_TARGET, PWM_MAX, PWM_MIN); // Inicializamos PID
	
	
	// Variables temporales
	float QEI_Actual_F;
	float PWM_Caculado_F;
	float PWM_Entero;
	
	while(1){
			QEI_Actual_F = (float)READ_QEI0Position(); // Leemos QEI y convertismo en flotante
			PWM_Caculado_F = PID_Compute(&HelicopterHelicopter, QEI_Actual_F);
			PWM_Entero = (uint16_t)PWM_Caculado_F;
		
			if(PWM_Entero < PWM_MIN)
					PWM_Entero = PWM_MIN;
			if(PWM_Entero > PWM_MAX)
					PWM_Entero = PWM_MAX;

			PWMA_M0_Set_Duty(PWM_Entero);
			
			MAX7219_Transmite_Number((uint32_t)QEI_Actual_F);
	}
}