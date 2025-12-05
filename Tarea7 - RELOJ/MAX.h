#ifndef MAX_H
#define	MAX_H 

// Evita múltiples inclusiones del archivo
#include "tm4c1294ncpdt.h"      // Definiciones del microcontrolador Tiva C
#include <stdint.h>             // Tipos enteros estándar

// Registros del MAX7219.
#define MAX7219_REG_NOOP        0x00    // No operación
#define MAX7219_REG_DIGIT0      0x01    // Dígito 0
#define MAX7219_REG_DIGIT1      0x02    // Dígito 1
#define MAX7219_REG_DIGIT2      0x03    // Dígito 2
#define MAX7219_REG_DIGIT3      0x04    // Dígito 3
#define MAX7219_REG_DIGIT4      0x05    // Dígito 4
#define MAX7219_REG_DIGIT5      0x06    // Dígito 5
#define MAX7219_REG_DIGIT6      0x07    // Dígito 6
#define MAX7219_REG_DIGIT7      0x08    // Dígito 7
#define MAX7219_REG_DECODEMODE  0x09    // Modo de decodificación BCD
#define MAX7219_REG_INTENSITY   0x0A    // Control de brillo
#define MAX7219_REG_SCANLIMIT   0x0B    // Cantidad de dígitos activos
#define MAX7219_REG_SHUTDOWN    0x0C    // Encender/apagar el display
#define MAX7219_REG_DISPLAYTEST 0x0F    // Modo de prueba
#define CHAR_DASH               0x0A    // Código para mostrar guion '-'

// Prototipos de funciones
void SPI_Init(void);                   // Inicializa SPI
void MAX7219_Init(void);               // Configura el MAX7219
void Transmite(uint8_t Reg, uint8_t Data);   // Envía registro y dato al MAX7219
void Transmite_Number(uint32_t Number);       // Envía un número completo al display
void ZERO(void);                        // Limpia o apaga todos los dígitos

#endif
