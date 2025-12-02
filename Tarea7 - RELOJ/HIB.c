#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "HIB.h"

//FUNCIONES HIB
void HIB_Init(void) {
    SYSCTL_RCGCHIB_R |= 0x01;          // Habilita el reloj para el módulo HIB
    while((SYSCTL_PRHIB_R & 0x01) == 0){}; // Espera a que el módulo esté listo

    if ((HIB_CTL_R & 0x00000040) == 0) {   // Si el RTC no está habilitado
        while((HIB_CTL_R & 0x80000000) == 0) {}; // Espera a que HIB esté listo para escribir
        HIB_CTL_R |= 0x00000040;         // Habilita el contador RTC
    }
    if ((HIB_CTL_R & 0x00000001) == 0) {   // Si la alimentación del HIB no está habilitada
        while((HIB_CTL_R & 0x80000000) == 0) {}; // Espera disponibilidad de escritura
        HIB_CTL_R |= 0x00000001;         // Habilita el módulo HIB
    }
}

void HIB_WriteTime(uint32_t nuevos_segundos) {
    while((HIB_CTL_R & 0x80000000) == 0) {}; // Espera a que HIB permita escritura  
    HIB_RTCLD_R = nuevos_segundos;       // Carga los nuevos segundos en el contador RTC
    while((HIB_CTL_R & 0x80000000) == 0) {}; // Espera a que termine la carga
}

uint32_t RTC32_Get(void){
    return HIB_DATA_R;                  // Devuelve el valor almacenado en HIB_DATA
}
