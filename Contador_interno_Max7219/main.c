

#include "stm32f10x.h"                  // Device header

//registros

#define RCC_APB2ENR (*(volatile uint32_t*)0x40021018) //registro de encendidos perifericos
	
// GPIOx registros
#define GPIOA_CRH   (*(volatile uint32_t*)0x40010804)  // configuraci�n de pines PA8..PA15
#define GPIOA_ODR   (*(volatile uint32_t*)0x4001080C)  // salida de datos (ODR)
#define TIM1_CR1    (*(volatile uint32_t*)0x40012C00)  // control principal (CEN, DIR, etc.)
	

//apuntador a contador
#define TIM1_CNT    (*(volatile uint32_t*)0x40012C24)  // contador actual
	

extern void conf_clk(void); //funcion que configura el reloj
extern void conf_spi(void); //funcion que configura la comunicacion spi
extern void Transmite(uint8_t reg, uint8_t dato); //funcion para transmitir el dato al max7219
extern void config_TIM(void); // configuracion del contador
extern void apagarLEDS(void);

void config_ports(void); //prototipo del confiiguracion de puerto
void TransmicionDeco(uint64_t variable); //variable de la transformacion del contador

void config_ports(void){
    GPIOA_CRH &= ~(0xF << 4);  // Limpiar configuraci�n de PA9
		GPIOA_CRH |=  (0x8 << 4);  // 1000b -> entrada con pull-up/down
		GPIOA_ODR |= (0 << 9);     // Pull-up activado (PA9 = HIGH por defecto)
}

/*
	Esta funcion simplemente decodifica el valor del contador entre unidades, decenas y centenas
	para luego llamar a funcion transmite e imprimir ese valor, y ya luego actualiza le misma variable
	sin el valor inicial
*/
void TransmicionDeco(uint64_t variable){
    int residuo; //variable para guardar residuo
		int NumDigito = 1;  //iniciamos en el digito 1
    do{    //inciamos loop
        residuo = variable % 10; //calculamos el residuo
        Transmite(NumDigito, residuo); // transmitimos ese residuo seria la unidad en un principio
        variable /= 10; // la misma variable la dividimos entre 10 para eliminar el diigito que escribimos
        NumDigito++; // nos movemos de digito del max
    }while(variable > 0); // para finalizar el loop
}


int main(void) {
    conf_clk(); //configuramos el reloj
    conf_spi(); //configuramos la comunicacion spi con el max7219
		//config_ports(); //no lo usamos pq al final la defini en push_button_timer.c
		config_TIM();//configuramos el reloj en el chanal 2 del A9
	
   while (1) {
		 
		
        // loop infinito
			uint16_t contador = TIM1_CNT; //asignamos el contador a una variable
		 if (contador == 0 ){
			 apagarLEDS();
		 }
		 /*else if (contador == 65535){
			TIM1_CR1 &= ~1;  // apaga el contador
			for (int a = 0; a< 8; a++)
			Transmite(a, 0x00);
    }*/ else{
			//TIM1_CR1 &= ~1;  // apaga el contador
				TransmicionDeco(contador); //se la mandamos a nuestra funcion decoder para mandar datos al max7219
			//for (int s=0; s<200; s++){}
			 // --- Reactivar el contador ---
			//TIM1_CR1 |= 1;   // vuelve a arrancar
		}
	 
	}	
}