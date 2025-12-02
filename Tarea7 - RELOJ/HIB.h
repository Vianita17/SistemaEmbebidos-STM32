#ifndef HIB_H
#define HIB_H

#include <stdint.h>
void HIB_Init(void); //INICALIZA EL MODULO hib
void HIB_WriteTime(uint32_t nuevos_segundos); // el valor del contador se actualiza
uint32_t RTC32_Get(void); //retorna lo que exista en HIB_DATA_R

#endif