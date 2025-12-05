#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "MAX.h" 	//header configuracion SSI
#include "HIB.h"	//header configuracion HIB
#include "GPIOS.h"	//header configuracion GPIOS


/*			CONEXIONs
		PL0 -> SET_MASK
		PL1 -> UP_MASK
		PL2 -> DOWN_MASK
		PL3 -> FORMAT

		PA2 -> CLK
		PA3 -> CS
		PA4 -> DIN
*/


// ----- RELOJ ESTADOS -----
#define MODO_NORMAL   0 //estado modo normal
#define MODO_SET_HORA 1 //estado modo config horas
#define MODO_SET_MIN  2 //estado modo config minutos
#define MODO_SET_SEG  3 //estado modo config segundos

// Variables Globales
uint32_t currently_seconds = 0;
uint8_t clock_status = MODO_NORMAL;
uint32_t currently_date;

// Variables de edición
uint32_t HOURS_EDIT = 0;
uint32_t MINUTES_EDIT = 0;
uint32_t SECONDS_EDIT = 0;

uint8_t format_24h = 1;   // 1 = formato 24h, 0 = formato 12h
uint8_t am_pm = 0;         // 0 = AM, 1 = PM

// Variable para controlar el tiempo entre accesos al SET
uint32_t last_set_time = 0; 

uint8_t READ_BUTTON(void); //funcion de lectura de botones

int main(void) {
		
	HIB_Init(); //inciallizacion HIB
  MAX7219_Init(); //inicizalizacion MAX
    
  Button_config(); //configuracion botones
  LEDS_config();	//configuracione LEDs

  uint32_t HOURS, MINUTES, SECONDS; //variables
  uint32_t DISPLAY;	//variable para guardar hora completa
  uint8_t btn_press = 0; //estado del boton

  if(HIB_DATA_R != 0) {
      HIB_WriteTime(HIB_DATA_R); //cargar la ultima hora guardada
  }

   // Inicializar last_set_time con el tiempo actual
  last_set_time = HIB_RTCC_R;
	uint8_t ignore_set = 0; //bandera de ignorar SET

    while(1) {
        btn_press = READ_BUTTON(); //checo botones
        switch(clock_status) {
            //MODO NORMAL
            case MODO_NORMAL:
								
								GPIO_PORTN_DATA_R &= ~LED2; //DECLRO EN MODO BAJO EL LED

                currently_seconds = HIB_RTCC_R;
						//CONVERSIONES DE LAS HORAS, SEGUNDOS,MINUTOS  DE TODA LA VIDA
                uint32_t DAY_SECONDS = currently_seconds % 86400;
                HOURS = DAY_SECONDS / 3600;
                MINUTES = (DAY_SECONDS % 3600) / 60;
                SECONDS = DAY_SECONDS % 60;

                //determinar AM/PM si estamos en formato 12
                if(!format_24h) {
                    if(HOURS == 0) {
                        HOURS = 12;
                        am_pm = 0;  // AM
                    }
                    else if(HOURS < 12) {
                        am_pm = 0;  // AM
                    }
                    else if(HOURS == 12) {
                        am_pm = 1;  // PM
                    }
                    else { // 13–23
                        HOURS -= 12;
                        am_pm = 1;  // PM
                    }
                }

                // edicion
                if(btn_press == 1 && !ignore_set) {
                    uint32_t tiempo_actual = HIB_RTCC_R;     
                    // 30 segundos desde la última vez
                    if((tiempo_actual - last_set_time) >= 30) {
                        last_set_time = tiempo_actual;
                        //ESTABLECEMOS LAS HORAS ACTUALES A EDITAR
                        HOURS_EDIT = HOURS;
                        MINUTES_EDIT = MINUTES;
                        SECONDS_EDIT = SECONDS;
                        clock_status = MODO_SET_HORA;   
                    }
                }

                // 
                //CAMBIO DE FORMATO 24h/12h
                if(btn_press == 4 && clock_status == MODO_NORMAL) {
                    if(format_24h == 1) {
                        // 24h ? 12h
                        format_24h = 0;
                        GPIO_PORTN_DATA_R ^= LED1;

                        if(HOURS == 0) {
                            HOURS = 12;
                            am_pm = 0; // AM
                        }
                        else if(HOURS < 12) {
                            am_pm = 0; // AM
                        }
                        else if(HOURS == 12) {
                            am_pm = 1; // PM
                        }
                        else {
                            HOURS -= 12;
                            am_pm = 1; // PM
                        }
                    }
                    else {
                        // 12h ? 24h
                        format_24h = 1;
                        GPIO_PORTN_DATA_R ^= LED1;

                        if(am_pm == 0) { // AM
                            if(HOURS == 12) {
                                HOURS = 0;  // 12 AM ? 00
                            }
                        } else { // PM
                            if(HOURS != 12) {
                                HOURS += 12; // 1–11 PM ? 13–23
                            }
                        }
                    }
                }

                break;
            
						//MODO SET HORA
            case MODO_SET_HORA:
								//GPIO_PORTN_DATA_R |= LED2; //ENCIENDO LED EN MODO EDICION
                if(btn_press == 2) { 
                    HOURS_EDIT++; //AUMENTAMOS HORAS 
                    if(format_24h) {
                        if(HOURS_EDIT > 23) HOURS_EDIT = 0; //FORMATO 24h 
                    } else {
                        if(HOURS_EDIT > 12) HOURS_EDIT = 1; //FORMATO 12h
                    }
                }

                if(btn_press == 3) {
                    if(format_24h) {
                        if(HOURS_EDIT == 0) HOURS_EDIT = 23;
                        else HOURS_EDIT--; //DISMINUIMOS HORAS EN FORMATO 24h
                    }
                    else {
                        if(HOURS_EDIT == 1) HOURS_EDIT = 12;
                        else HOURS_EDIT--; //DISMINUIMOS HORAS EN FORMATO 12h
                    }
                }

                if(btn_press == 1) clock_status = MODO_SET_MIN; //CAMBIAMOS DE ESTADO
								//GUARDAMOS HOARS EDITADAS
                HOURS = HOURS_EDIT; 
                MINUTES = MINUTES_EDIT;
                SECONDS = SECONDS_EDIT;
                break;

            //MODO SET MIN
            case MODO_SET_MIN:
                if(btn_press == 2) {
                    MINUTES_EDIT++; //AUMENTAMOS MINUTOS EN CUALQUIER FORMATO
                    if(MINUTES_EDIT > 59) MINUTES_EDIT = 0;
                }
                if(btn_press == 3) {
                    if(MINUTES_EDIT == 0) MINUTES_EDIT = 59;
                    else MINUTES_EDIT--; //DISMINUIMOS MINUTOS EN CUALQUIER FORMATO
                }
                if(btn_press == 1) clock_status = MODO_SET_SEG; //CAMBIAMOS A SEGUNDOS
									//GUARDAMOS VALORES
                HOURS = HOURS_EDIT;
                MINUTES = MINUTES_EDIT;
                SECONDS = SECONDS_EDIT;
                break;

            // MODO SET SEG
            case MODO_SET_SEG:

                if(btn_press == 2) {
                    SECONDS_EDIT++; //AUMENTAMOS SEGUNDOS EN CUALQUIER FORMATO
                    if(SECONDS_EDIT > 59) SECONDS_EDIT = 0;
                }
                if(btn_press == 3) {
                    if(SECONDS_EDIT == 0) SECONDS_EDIT = 59;
                    else SECONDS_EDIT--; //DISMINUIMOS SEGUNDOS EN CUALQUIER FORMATO
                }

                if(btn_press == 1) {

                    uint32_t h24 = HOURS_EDIT;

                    // Convertir edición si estamos en 12h
                    if(!format_24h) {
                        if(am_pm == 0) { // AM
                            if(h24 == 12) h24 = 0;
                        } else {
                            if(h24 != 12) h24 += 12;
                        }
                    }
										//GUARDAMOS VALORES EDITADOS
                    uint32_t NOW_COUNT =
                        (h24 * 3600) +
                        (MINUTES_EDIT * 60) +
                        SECONDS_EDIT;

                    HIB_WriteTime(NOW_COUNT); //SOBREESCRIBIMOS HIBRTCLD
										last_set_time = HIB_RTCC_R; //GUARDAMOS EL TIEMPO EN OTRA VARIABLE
										ignore_set = 1;//ACTIVAMOS LA BANDERA DE IGNORAR
										HIB_DATA_R = HIB_RTCC_R; //GUARDAMOS EL VALOR EN DATA

                    clock_status = MODO_NORMAL; //VOLVEMOS AL MODO NORMAL
										
										
                }
								
								//ESTABLECEMOS LOS VALORES YA EDITADOS
                HOURS = HOURS_EDIT;
                MINUTES = MINUTES_EDIT;
                SECONDS = SECONDS_EDIT;
                break;
        }

        // GUARDO LOS VALORES DE HORAS,SEGUNDOS,MINUTOS en una variable
        DISPLAY =
            (HOURS * 10000) +
            (MINUTES * 100) +
            (SECONDS);

        Transmite_Number(DISPLAY); //MUESTRO LA HORA EN EL MAX
				HIB_DATA_R = HIB_RTCC_R; //GUARDO EL VALOR DE LA HORA
				// Apagar la bandera después de un ciclo
				ignore_set = 0;

    }
}

uint8_t READ_BUTTON(void) {
		//GUARDAMOS CADENAS DE TODOS LOS PINES
    uint32_t data = GPIO_PORTL_DATA_R & (BTN_SET_MASK | BTN_UP_MASK | BTN_DOWN_MASK | BTN_FORMAT | BTN_CALENDAR);
    //SI NINGUNO ESTA ACTIVO 
		if (data == (BTN_SET_MASK | BTN_UP_MASK | BTN_DOWN_MASK | BTN_FORMAT | BTN_CALENDAR)) return 0;
    for(int i=0; i<40000; i++){} //DEBOUNCE PRIMIVITO
    //SIMPLIFICAMOS TODO EN UNA VARIABLE PARA NO DEJAR GRANDE
			//LA CONDICION IF
		data = GPIO_PORTL_DATA_R & (BTN_SET_MASK | BTN_UP_MASK | BTN_DOWN_MASK | BTN_FORMAT | BTN_CALENDAR);
		//CHECAMOS CADA UNO DE LOS PINES SI ESTAN EN CEROS (PRESS)
    if ((data & BTN_SET_MASK) == 0) { 
        while((GPIO_PORTL_DATA_R & BTN_SET_MASK) == 0){};
        return 1; // SET
    }
    if ((data & BTN_UP_MASK) == 0) {
        while((GPIO_PORTL_DATA_R & BTN_UP_MASK) == 0){};
        return 2; // UP
    }
    if ((data & BTN_DOWN_MASK) == 0) {
        while((GPIO_PORTL_DATA_R & BTN_DOWN_MASK) == 0){};
        return 3; // DOWN
    }
    if ((data & BTN_FORMAT) == 0) {
        while((GPIO_PORTL_DATA_R & BTN_FORMAT) == 0){};
        return 4; // FORMAT
    }
    if ((data & BTN_CALENDAR) == 0) {
        while((GPIO_PORTL_DATA_R & BTN_CALENDAR) == 0){};
        return 5; // CALENDAR
    }
    return 0;
}