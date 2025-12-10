#include <stdint.h>
// Incluye el tipo de datos estandarizado de enteros (por ejemplo, uint32_t).

// ------------------- DEFINICIONES DE REGISTROS -------------------
#define SYSCTL_RCGCGPIO_R   (*(volatile uint32_t *)0x400FE608)
// Registro de Control de Reloj de Periféricos GPIO.
// Se usa para habilitar el reloj (y, por lo tanto, la alimentación) del puerto GPIO.

#define SYSCTL_PRGPIO_R     (*(volatile uint32_t *)0x400FEA08)
// Registro de Disponibilidad de Periféricos GPIO (Peripheral Ready).
// Se usa para verificar que el reloj del GPIO esté estable antes de acceder a sus registros.

#define SYSCTL_RCGCTIMER_R  (*(volatile uint32_t *)0x400FE604) 
// Registro de Control de Reloj de Periféricos de Timer.
// Se usa para habilitar el reloj del módulo de Timer.

// Puerto F (LED)
#define GPIOF_PORTF_DATA_R  (*(volatile uint32_t *)0x4005D004)
// Registro de Datos del Puerto F. Se usa para leer o escribir el estado lógico de los pines.

#define GPIOF_PORTF_DIR_R   (*(volatile uint32_t *)0x4005D400)
// Registro de Dirección del Puerto F. Controla si un pin es entrada (0) o salida (1).

#define GPIOF_PORTF_AFSEL_R (*(volatile uint32_t *)0x4005D420)
// Registro de Selección de Función Alternativa. Controla si un pin es GPIO (0) o función alternativa (1).

#define GPIOF_PORTF_DEN_R   (*(volatile uint32_t *)0x4005D51C)
// Registro de Habilitación Digital (Digital Enable). Habilita la función digital de un pin (1).

// Timer 0
#define TIMER0_CFG_R        (*(volatile uint32_t *)0x40030000)
// Registro de Configuración del Timer 0. Define el tamaño y el modo del timer (ej. 16/32 bits).

#define TIMER0_TAMR_R       (*(volatile uint32_t *)0x40030004)
// Registro de Modo del Timer A. Define si opera en modo Periódico, Captura, One-Shot, etc.

#define TIMER0_CTL_R        (*(volatile uint32_t *)0x4003000C)
// Registro de Control del Timer. Se usa para habilitar (TAEN) o deshabilitar el timer.

#define TIMER0_IMR_R        (*(volatile uint32_t *)0x40030018)
// Registro de Máscara de Interrupción. Habilita o deshabilita la generación de interrupciones.

#define TIMER0_RIS_R        (*(volatile uint32_t *)0x4003001C)
// Registro de Estado Bruto de Interrupción (Raw Interrupt Status). Contiene la bandera de desbordamiento (timeout).

#define TIMER0_ICR_R        (*(volatile uint32_t *)0x40030024)
// Registro de Limpieza de Interrupción (Interrupt Clear). Se usa para borrar las banderas de interrupción.

#define TIMER0_TAILR_R      (*(volatile uint32_t *)0x40030028)
// Registro de Valor de Carga Inicial del Timer A. Establece el valor de cuenta regresiva.

#define TIMER0_TAPR_R       (*(volatile uint32_t *)0x40030038)
// Registro del Prescaler del Timer A. Permite reducir la frecuencia de reloj del timer.

void Timer0A_Init(uint32_t period);
void GPIOF_Init(void);

void GPIOF_Init(void){
    SYSCTL_RCGCGPIO_R |= 0x00000020;        
    // Habilita el reloj para el Puerto F (Bit 5 = 0x20) en el RCGCGPIO.
    while(!(SYSCTL_PRGPIO_R & 0x00000020)); 
    // Espera (polling) hasta que el reloj del Puerto F esté listo (Bit 5 en PRGPIO).
    GPIOF_PORTF_DEN_R |= 0x01;          
    // Habilita la función digital para el Pin 0 (0x01).
    GPIOF_PORTF_DIR_R |= 0x01;          
    // Establece el Pin 0 como SALIDA (1) en el registro de Dirección.
    GPIOF_PORTF_AFSEL_R &= ~0x01;            // Deshabilita la Función Alternativa para el Pin 0 (0), asegurando que sea GPIO.
}




void Timer0A_Init(uint32_t period){
    volatile uint32_t delay;
    // Variable auxiliar para introducir un retardo de bus necesario tras habilitar el reloj.

    SYSCTL_RCGCTIMER_R |= 0x01; 
    // Habilita el reloj para el Timer 0 (Bit 0 = 0x01) en el RCGCTIMER.

    delay = SYSCTL_RCGCTIMER_R; 
    // Lectura del registro para introducir un breve retardo (2 a 5 ciclos de reloj del sistema).

    (void)delay;    
    // Evita advertencias de compilador sobre la variable 'delay' no utilizada.

    TIMER0_CTL_R = 0x00000000;      
    // DESHABILITA el Timer 0A (TAEN=0) para poder configurarlo.

    TIMER0_CFG_R = 0x00000000;      
    // Configura el Timer 0 para un modo de 32 bits (bits [2:0]=0x0).

    TIMER0_TAMR_R = 0x00000002;     
    // Configura el Sub-Timer A: Modo Periódico (TAMR=0x2) y Cuenta Regresiva (TACDIR=0, por defecto).

    TIMER0_TAILR_R = period - 1;    
    // Establece el valor de carga inicial (period). El contador cuenta desde 'period - 1' hasta 0.

    TIMER0_TAPR_R = 0;          
    // Establece el Prescaler del Timer A a 0 (no se usa prescaler adicional).

    TIMER0_ICR_R = 0x00000001;      
    // Limpia cualquier bandera de 'Timeout' (TATOICR=1) que pudiera estar pendiente.

    TIMER0_IMR_R = 0x00000000;  
    // DESHABILITA las interrupciones (TAIM=0). Esto es clave para trabajar con **Polling**.

    TIMER0_CTL_R = 0x00000001;      
    // HABILITA el Timer 0A (TAEN=1) para que comience la cuenta regresiva.
}



int main(void){
    GPIOF_Init();   
    // Llama a la función para configurar el Pin F0 como salida digital.

    // CORRECCIÓN APLICADA:
    // Para 4 Hz (0.25s por ciclo completo), necesitamos cambiar de estado cada 0.125s
    // 16,000,000 * 0.125 = 2,000,000
    // Lógica: Frecuencia de parpadeo = 4 Hz. Período del parpadeo (LED ON + LED OFF) = 1/4 = 0.25s.
    // Para cambiar el estado del LED (ON -> OFF o OFF -> ON) se requiere la mitad del período: 0.25s / 2 = 0.125s.
    // La frecuencia del reloj del sistema/timer es 16 MHz.
    // Conteo = Tiempo * Frecuencia = 0.125s * 16,000,000 Hz = 2,000,000.

    Timer0A_Init(2000000);  
    // Inicializa el Timer 0A para generar un evento de "timeout" cada 0.125 segundos.

    while(1){
    // Bucle infinito (Superloop) donde se ejecuta la lógica del programa.

        if(TIMER0_RIS_R & 0x00000001) {
        // **Polling**: Verifica el bit TATORIS (Bit 0) en el Registro de Estado Bruto de Interrupción (RIS).
        // Si el bit es 1, significa que el Timer A ha llegado a cero y ha ocurrido un "timeout".

            TIMER0_ICR_R = 0x00000001;  // Bajar bandera
            // Escribiendo 1 en el bit TATOICR (Bit 0) del Registro ICR, se LIMPIA (baja) la bandera de "timeout".

            GPIOF_PORTF_DATA_R ^= 0x01; // Invertir LED
            // Utiliza la operación **XOR** (`^=`) con el bit 0x01 para invertir el estado del Pin 0.
            // Si estaba en 0 (OFF), pasa a 1 (ON). Si estaba en 1 (ON), pasa a 0 (OFF).
        }
    }
}