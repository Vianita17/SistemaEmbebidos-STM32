// PWM.c                                                     // Comentario general del archivo
#include <stdint.h>                                          // Tipos enteros estándar
#include "tm4c1294ncpdt.h"                                   // Definiciones de registros TM4C1294
#include "PWM.h"                                             // Prototipos del módulo PWM

// ------------ PWM0A en PF0 ------------                      // Título de sección PWM0A
void PWM0A_Init(uint16_t period, uint16_t duty){             // Configura PWM0A (PF0)

  SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0;                     // Habilita reloj al módulo PWM0
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;                   // Habilita reloj al Puerto F
  while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) == 0){};        // Espera a que Port F esté listo

  // PF0 como M0PWM0                                          // Comentario de función de pin
  GPIO_PORTF_AFSEL_R |= 0x01;                                // PF0 en función alterna
  GPIO_PORTF_DEN_R   |= 0x01;                                // Habilita PF0 como digital
  GPIO_PORTF_PCTL_R   = (GPIO_PORTF_PCTL_R & 0xFFFFFFF0) | 0x00000006; // PCTL: PF0->PWM
  GPIO_PORTF_AMSEL_R &= ~0x01;                               // Desactiva analógico en PF0

  while((SYSCTL_PRPWM_R & SYSCTL_PRPWM_R0) == 0){};          // Espera a que PWM0 esté listo

	// Usar reloj del sistema directamente (16 MHz, sin divisor)
	PWM0_CC_R &= ~PWM_CC_USEPWM;   // USEPWM = 0 -> ignora PWMDIV, usa el clock base


  PWM0_0_CTL_R = 0;                                          // Apaga generador 0 para configurar

  // PF0=0 en LOAD, PF0=1 en CMPA (modo down-count)           // Comentario de modo salida
  PWM0_0_GENA_R = (PWM_0_GENA_ACTLOAD_ZERO |                 // Acción al cargar LOAD: salida a 0
                   PWM_0_GENA_ACTCMPAD_ONE);                 // Acción al comparar CMPA: salida a 1

  PWM0_0_LOAD_R = period - 1;                                // Carga periodo (LOAD = period-1)
  PWM0_0_CMPA_R = duty   - 1;                                // Carga duty (CMPA = duty-1)

  PWM0_0_CTL_R   |= PWM_0_CTL_ENABLE;                        // Habilita generador 0
  PWM0_ENABLE_R  |= PWM_ENABLE_PWM0EN;                       // Habilita salida PWM0 (PF0)
}

void PWM0A_Duty(uint16_t duty){                              // Actualiza duty de PWM0A
  if(duty < 2) duty = 2;                                     // Evita valores demasiado chicos
  PWM0_0_CMPA_R = duty - 1;                                  // Escribe nuevo CMPA (duty-1)
}

// ------------ PWM0B en PF1 (opcional) ------------           // Título de sección PWM0B
void PWM0B_Init(uint16_t period, uint16_t duty){             // Configura PWM0B (PF1)

  SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0;                     // Habilita reloj al módulo PWM0
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;                   // Habilita reloj al Puerto F
  while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) == 0){};        // Espera a que Port F esté listo

  // PF1 como M0PWM1                                          // Comentario de función de pin
  GPIO_PORTF_AFSEL_R |= 0x02;                                // PF1 en función alterna
  GPIO_PORTF_DEN_R   |= 0x02;                                // Habilita PF1 como digital
  GPIO_PORTF_PCTL_R   = (GPIO_PORTF_PCTL_R & 0xFFFFFF0F) | 0x00000060; // PCTL: PF1->PWM
  GPIO_PORTF_AMSEL_R &= ~0x02;                               // Desactiva analógico en PF1

  while((SYSCTL_PRPWM_R & SYSCTL_PRPWM_R0) == 0){};          // Espera a que PWM0 esté listo

	// Usar reloj base del sistema (16 MHz, sin divisor)
  PWM0_CC_R &= ~PWM_CC_USEPWM;   // USEPWM = 0 -> ignora PWMDIV, usa clock base	
	/*
  PWM0_CC_R |=  PWM_CC_USEPWM;                               // Habilita uso de divisor PWMDIV
  PWM0_CC_R &= ~PWM_CC_PWMDIV_M;                             // Limpia campo divisor
  PWM0_CC_R |=  PWM_CC_PWMDIV_64;                            // Selecciona divisor /64
	*/
  PWM0_0_CTL_R = 0;                                          // Apaga generador 0 para configurar

  PWM0_0_GENB_R = (PWM_0_GENB_ACTLOAD_ZERO |                 // Acción al cargar LOAD: salida a 0
                   PWM_0_GENB_ACTCMPBD_ONE);                 // Acción al comparar CMPB: salida a 1

  PWM0_0_LOAD_R = period - 1;                                // Carga periodo (LOAD = period-1)
  PWM0_0_CMPB_R = duty   - 1;                                // Carga duty (CMPB = duty-1)

  PWM0_0_CTL_R   |= PWM_0_CTL_ENABLE;                        // Habilita generador 0
  PWM0_ENABLE_R  |= PWM_ENABLE_PWM1EN;                       // Habilita salida PWM1 (PF1)
}

void PWM0B_Duty(uint16_t duty){                              // Actualiza duty de PWM0B
  if(duty < 2) duty = 2;                                     // Evita valores demasiado chicos
  PWM0_0_CMPB_R = duty - 1;                                  // Escribe nuevo CMPB (duty-1)
}
