

#ifndef QEI_H
#define QEI_H

#include <stdint.h>
#include "tm4c1294ncpdt.h"

void QEI_Init(uint32_t MaxPosition);
uint32_t READ_QEI0Position(void);

#endif
