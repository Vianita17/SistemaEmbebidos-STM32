#include <stdint.h>                         // Tipos enteros estándar
#include "tm4c1294ncpdt.h"                  // Definiciones de registros de la TM4C1294
#include "QEI.h"                            // Prototipos del módulo QEI

void QEI_Init(void){                        // Función que configura QEI0 en modo cuadratura

    SYSCTL_RCGCQEI_R  |= SYSCTL_RCGCQEI_R0; // Habilita reloj al módulo QEI0
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R10; // Habilita reloj al puerto L
    while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R10)==0){} // Espera a que Port L esté listo

    GPIO_PORTL_AFSEL_R |= 0x0E;             // Activa función alterna en PL1, PL2, PL3
    GPIO_PORTL_PCTL_R   = (GPIO_PORTL_PCTL_R & 0xFFFF000F) | 0x00006660; // Mapea PL1-3 a QEI
    GPIO_PORTL_DEN_R   |= 0x0E;             // Habilita digital en PL1, PL2, PL3

    while((SYSCTL_PRQEI_R & SYSCTL_PRQEI_R0)==0){} // Espera a que QEI0 esté listo

    QEI0_CTL_R &= ~QEI_CTL_ENABLE;          // Deshabilita QEI para configurarlo seguro

    QEI0_CTL_R &= ~QEI_CTL_FILTCNT_M;       // Limpia campo de conteo del filtro
    QEI0_CTL_R |=  QEI_CTL_FILTEN;          // Activa el filtro antirruido

    QEI0_CTL_R &= ~QEI_CTL_VELEN;           // Desactiva medición de velocidad
    QEI0_CTL_R &= ~QEI_CTL_RESMODE;         // Selecciona reset por MAXPOS (no por IDX)

    QEI0_CTL_R &= ~QEI_CTL_SIGMODE;         // SIGMODE=0 -> modo cuadratura PhA/PhB
    QEI0_CTL_R |=  QEI_CTL_CAPMODE;         // CAPMODE=1 -> cuenta flancos A y B

    QEI0_MAXPOS_R = 4095;                   // Define el valor máximo del contador
    QEI0_POS_R = 0;                         // Inicializa el contador en 0

    QEI0_CTL_R |= QEI_CTL_ENABLE;           // Habilita el QEI ya configurado
}

uint16_t QEI_pos(void){             // Función para leer la posición actual
    return (uint16_t)QEI0_POS_R;            // Retorna los 16 bits bajos del contador
}
