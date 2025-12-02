#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "MAX.h"
#include "HIB.h"

//PUERTO L
#define BTN_SET_MASK  (1 << 0) // PL0
#define BTN_UP_MASK   (1 << 1) // PL1
#define BTN_DOWN_MASK (1 << 2) // PL2

#define FORMAT 23
//#define FORMAT 12
#define CHAR_DASH       0x0A // Guion (-)
// Estados del Reloj
#define MODO_NORMAL   0
#define MODO_SET_HORA 1
#define MODO_SET_MIN  2
#define MODO_SET_SEG  3  // <--- NUEVO ESTADO
// Variables Globales
uint32_t hora_actual_segundos = 0; 
uint8_t estado_reloj = MODO_NORMAL;

// Variables de edición
uint32_t edit_horas = 0;
uint32_t edit_minutos = 0;
uint32_t edit_segundos = 0; // <--- VARIABLE PARA SEGUNDOS

// Prototipos
void PortL_Buttons_Init(void);
uint8_t LeerBotones(void);

int main(void) {
    // 1. Inicializar Hardware
    MAX7219_Init(); //iniciar mcomunicacion spi
    HIB_Init(); //iniciar HIB
    PortL_Buttons_Init(); //configurar puertos 

    // Variables locales para visualización
    uint32_t display_h, display_m, display_s; 
    uint32_t numero_para_display;
    uint8_t btn_presionado = 0;

    //uint32_t GET = RTC32_Get();
		if(HIB_DATA_R != 0) {
				HIB_WriteTime(HIB_DATA_R);
		}

    while(1) {
        // --- A. LEER BOTONES ---
        btn_presionado = LeerBotones(); 
				HIB_DATA_R = HIB_RTCC_R;

        // --- B. MÁQUINA DE ESTADOS ---
        switch(estado_reloj) {
            
            // --- MODO 0: NORMAL (VER HORA) ---
            case MODO_NORMAL:
                hora_actual_segundos = HIB_RTCC_R;
                uint32_t seg_dia = hora_actual_segundos % 86400;
                
                display_h = seg_dia / 3600;
                display_m = (seg_dia % 3600) / 60;
                display_s = seg_dia % 60;

                if(btn_presionado == 1) { // SET -> Entrar a editar
                    edit_horas = display_h;
                    edit_minutos = display_m;
                    edit_segundos = display_s; // Cargamos los segundos actuales
                    estado_reloj = MODO_SET_HORA;
                }
                break;

            // --- MODO 1: EDITAR HORAS ---
            case MODO_SET_HORA:
                if(btn_presionado == 2) { // UP
                    edit_horas++;
                    if(edit_horas > FORMAT) edit_horas = 0;
                }
                if(btn_presionado == 3) { // DOWN
                    if(edit_horas == 0) edit_horas = FORMAT;
                    else edit_horas--;
                }
                if(btn_presionado == 1) { // SET -> Ir a Minutos
                    estado_reloj = MODO_SET_MIN;
                }
                display_h = edit_horas;
                display_m = edit_minutos;
                display_s = edit_segundos; 
                break;

            // --- MODO 2: EDITAR MINUTOS ---
            case MODO_SET_MIN:
                if(btn_presionado == 2) { // UP
                    edit_minutos++;
                    if(edit_minutos > 59) edit_minutos = 0;
                }
                if(btn_presionado == 3) { // DOWN
                    if(edit_minutos == 0) edit_minutos = 59;
                    else edit_minutos--;
                }
                if(btn_presionado == 1) { // SET -> Ir a Segundos (CAMBIO AQUI)
                    estado_reloj = MODO_SET_SEG;
                }
                display_h = edit_horas;
                display_m = edit_minutos;
                display_s = edit_segundos;
                break;

            // --- MODO 3: EDITAR SEGUNDOS (NUEVO) ---
            case MODO_SET_SEG:
                if(btn_presionado == 2) { // UP
                    edit_segundos++;
                    if(edit_segundos > 59) edit_segundos = 0;
                }
                if(btn_presionado == 3) { // DOWN
                    if(edit_segundos == 0) edit_segundos = 59;
                    else edit_segundos--;
                }
                if(btn_presionado == 1) { // SET -> GUARDAR TODO
                    // Calcular nueva hora completa
                    uint32_t nueva_cuenta = (edit_horas * 3600) + (edit_minutos * 60) + edit_segundos;
                    HIB_WriteTime(nueva_cuenta);
                    estado_reloj = MODO_NORMAL;
                }
                display_h = edit_horas;
                display_m = edit_minutos;
                display_s = edit_segundos;
                break;
        }

        // --- C. PANTALLA ---
        numero_para_display = (display_h * 10000) + (display_m * 100)  + display_s;
        Transmite_Number(numero_para_display);

        // Retardo para velocidad de botones
        for(int i=0; i<50000; i++){}
    }
}

// --- CONFIGURACIÓN DEL PUERTO L ---
void PortL_Buttons_Init(void) {
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R10; // Activar Port L
    while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R10) == 0){};

    GPIO_PORTL_DIR_R &= ~(BTN_SET_MASK | BTN_UP_MASK | BTN_DOWN_MASK); // Entradas
    GPIO_PORTL_DEN_R |= (BTN_SET_MASK | BTN_UP_MASK | BTN_DOWN_MASK);  // Digital
    GPIO_PORTL_PUR_R |= (BTN_SET_MASK | BTN_UP_MASK | BTN_DOWN_MASK);  // Pull-Up
}

uint8_t LeerBotones(void) {
    uint32_t data = GPIO_PORTL_DATA_R & (BTN_SET_MASK | BTN_UP_MASK | BTN_DOWN_MASK); //leo estados del puerto
	
    if (data == (BTN_SET_MASK | BTN_UP_MASK | BTN_DOWN_MASK)) return 0; // Nada presionado

    for(int i=0; i<40000; i++){} // Anti-rebote
    data = GPIO_PORTL_DATA_R & (BTN_SET_MASK | BTN_UP_MASK | BTN_DOWN_MASK); //actualizo la lectura

    if ((data & BTN_SET_MASK) == 0) { //configurar hh-mm-ss
        while((GPIO_PORTL_DATA_R & BTN_SET_MASK) == 0){}; 
        return 1; // SET
    }
    if ((data & BTN_UP_MASK) == 0) { //configurar digito up
        while((GPIO_PORTL_DATA_R & BTN_UP_MASK) == 0){};
        return 2; // UP
    }
    if ((data & BTN_DOWN_MASK) == 0) { //configurar digito down
        while((GPIO_PORTL_DATA_R & BTN_DOWN_MASK) == 0){};
        return 3; // DOWN
    }
    return 0;
}
