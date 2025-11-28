#include <stdint.h>              // Tipos enteros estándar (uint16_t, uint32_t, etc.)
#include "tm4c1294ncpdt.h"       // Mapa de registros y bits de la TM4C1294
#include "PWM.h"                 // Prototipos para inicializar/actualizar PWM
#include "QEI.h"                 // Prototipos para inicializar/leer QEI
#include "MAX7219.h"             // Prototipos para controlar el MAX7219

// QEI
#define QEI_MIN           0      // Posición mínima del encoder
#define QEI_MAX           935    // Posición máxima del encoder
#define QEI_MID           435    // Posición media (equilibrio)

//umbral de referencia
#define REF_LOW        405    // Límite bajo de banda ideal
#define REF_HIGH      469    // Límite alto de banda ideal

//PID 
#define KP    0.3f    //Ganancia Proporcional
#define KI    0.03f   // Ganancia integral
#define KD    4.0f    // Ganancia derivativa
#define DT    0.01f   // Paso de tiempo lógico -> 10ms

//PWM 
#define Fclk 16000000 //Frecuencia que esta trabajando el Reloj de la tiva
#define Fpwm 2000 //Frecuencia minima deseada

#define PWM_PERIOD  (uint16_t)(Fclk/Fpwm) //Periodo = Fclk/Fpwm

#define PWM_MIN    ((uint16_t)(PWM_PERIOD * 0.10f))  // Duty mínimo permitido (10%)
#define PWM_MAX    ((uint16_t)(PWM_PERIOD * 0.80f))  // Duty máximo permitido (80%)
#define PWM_EQ     ((uint16_t)(PWM_PERIOD * 0.35f))  // Duty “base” en equilibrio (~35%)

#define I_MAX  ((float)PWM_PERIOD * 0.25f)  // Límite anti-windup de integral

//VARIABLES GLOBALES

    float error, prev_error = 0.0f;                    // Error actual y anterior
    float integral = 0.0f;                             // Acumulador integral
    float derivative;                                 // Término derivativo
    float control;                                           // Salida del PID en float
    float duty_f;                                      // Duty calculado en float
    uint16_t duty;                                    // Duty final entero


//delay
static void Delay(void){                        // Retardo simple por software
    volatile uint32_t i;                                // Variable “volatile” para que no optimice
    for(i = 0; i < 40000; i++){}                        // Bucle vacío como delay
}
void ControlPID(uint16_t pos)
{
				// Error respecto al medio
        // pos baja -> error + -> duty sube 
        error = (float)((int32_t)QEI_MID - (int32_t)pos); // Calcula error (signo correcto)
				
				// Si ves que va al revés, usa esta:
        // error = (float)((int32_t)pos - (int32_t)ENC_MID);

        // 4) Banda ideal: 416..469 => error=0
        if(pos >= REF_LOW && pos <= REF_HIGH){   // Verifica si está en banda ideal
            error = 0.0f;                              // Fuerza error cero
            prev_error = 0.0f;                         // Resetea derivada para evitar patada
        }

        // (anti windup)
        integral += error * KI * DT;                  // Integra error escalado
        if(integral >  I_MAX) integral =  I_MAX;      // Limita integral superior
        if(integral < -I_MAX) integral = -I_MAX;      // Limita integral inferior

        // derivada
        derivative = (error - prev_error) / DT;       // Derivada discreta del error

        // PID
        control = KP*error + integral + KD*derivative;      // Calcula salida PID total
        prev_error = error;                           // Guarda error para siguiente ciclo

        // Duty
        duty_f = (float)PWM_EQ + control;            // Suma PID a duty base

        if(duty_f < (float)PWM_MIN) duty_f = (float)PWM_MIN; // Satura duty mínimo
        if(duty_f > (float)PWM_MAX) duty_f = (float)PWM_MAX; // Satura duty máximo

        duty = (uint16_t)duty_f;                      // Convierte duty a entero

        PWM0A_Duty(duty);                             // Actualiza PWM en PF0

}

int main(void){                                        // Función principal
		uint16_t Q_pos;                                      // Posición actual del encoder (0..935)
    QEI_Init();                                 // Inicializa QEI0 en cuadratura
		MAX7219_Init();                             // Inicializa MAX7219
    PWM0A_Init(PWM_PERIOD, PWM_EQ);             // Inicializa PWM0A en PF0
    // PWM0B_Init(PWM_PERIOD, PWM_EQ);          // PWM0B opcional (PF1)

    while(1){                                          // Lazo infinito
        //Leer posición del enconder
        Q_pos = QEI_pos();                       // Lee contador del QEI
        if(Q_pos > QEI_MAX) Q_pos = QEI_MAX;               // Satura a máximo real
        // Mostrar ceros
        Transmite_Number(Q_pos);                    // Muestra posición en 8 dígitos
        ControlPID(Q_pos);//Hacemos el control PID
        Delay();                              // Retardo para suavizar control
    }
}
