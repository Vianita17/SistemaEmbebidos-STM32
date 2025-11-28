// QEI.h
// QEI0 usando PL1 (PhA0) y PL2 (PhB0) en modo cuadratura

#ifndef QEI_H
#define QEI_H

#include <stdint.h>

// Inicializa QEI0 (PL1, PL2) en modo cuadratura
void QEI_Init(void);

// Lee la posición actual (registro QEI0_POS_R de 32 bits, retornamos 16)
uint16_t QEI_pos(void);

#endif
