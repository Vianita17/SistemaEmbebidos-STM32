
//author: Julian Adrian Viana Palomo
#include "stm32f10x.h"                  // Device header
#include "address.h"
#include "stdint.h"
#include "stdbool.h"
/*
	CCodigo de la logica principal donde vamos a controlar la deteccion de dos botones
		boton 1 -> B0
		boton 2 -> B1	
	Dependiendo cual boton se pulse vamos a mandar un valor atraves del pin TX para
	esperar que otro dispositivo encienda un led, con dos botones -> dos leds y viceversa
	
	conexion UART
	pin9 (TX) - pin10(Rx)
	pin10(Rx) - pin9(Tx)		
*/

extern void CLKconfig(void);
extern void USARTports(void);
extern void USARTconfig(void);
extern void transmmitt(uint8_t value);
uint8_t receiver(void);

void PINconfig(void){
	
	//RCC_APB2ENR |= (1<<3); //enciendo puerto B
	
	//configuramos los pines para los BOTONES
	GPIOB_CRL &= ~((0xF << 0)|(0xF << 4)); //limpio config, pines 0-1
	GPIOB_CRL |= (0x8 << 0); //pinB0 como input up/down
	GPIOB_CRL |= (0x8 << 4); //pinB1 como input up/down
	
	//configuramos los pines para los LEDS
	GPIOB_CRH &= ~((0xF << 16)|(0xF << 20)|(0xF << 24)); //limpio config, pines 12-13
	GPIOB_CRH |= (0x3 << 16); //pinB12 como Output pushpull 50Mhz
	GPIOB_CRH |= (0x3 << 20); //pinB13 como Output pushpull 50Mhz
	GPIOB_CRH |= (0x3 << 24); //pinB14 como output pushpull 50Mhz
	
	//Declaro todos los pines en estado bajo
	GPIOB_ODR &= ~(1 << 0);     // Pull-down
	GPIOB_ODR &= ~(1 << 1);     // Pull-down
	GPIOB_ODR &= ~(1 << 12);     // Led B12 OFF
	GPIOB_ODR &= ~(1 << 13);     // Led B13 OFF
	GPIOB_ODR &= ~(1 << 14);     // Led B14 OFF
}

static int last_state_b0 = 0;
static int last_state_b1 = 0;



int main (void){
	
	//configuramos el reloj HSI
	CLKconfig();
	
	USARTports();//configuramos los pines9 Tx y pin10 Rx
	
	//configuramos TX/RX
	USARTports();
	
	//cconfiguramos USART como protocolo UART
	USARTconfig();
	
	//configuramos LED/BOTONES
	PINconfig();
	
	while(1){//LOGICA PRINCIPAL
		
		//LECTURA DEl BOTON (cuando transmite)
		int b0 = (GPIOB_IDR & (1 << 0)) ? 1 : 0; // Botón B0
    int b1 = (GPIOB_IDR & (1 << 1)) ? 1 : 0; // Botón B1
		
		uint8_t data_to_send = 0x00;

    // Detectar flanco de subida (presión)
    if (b0 && !last_state_b0 && !b1) { 
        data_to_send = 0x01; // pinB0 presionadoh
    } 
    else if (b1 && !last_state_b1 && !b0) {
        data_to_send = 0x02; // pinB1 presionado
    } 
    else if (b0 && b1) {
        data_to_send = 0x03; //ambos presionados
    }

    //TRANSMISIÓN SOLO SI HAY CAMBIO
    if (data_to_send != 0x00) {
        transmmitt(data_to_send);
        for (volatile int i = 0; i < 50000; i++); // Pequeño retardo (debounce)
    }else{
				transmmitt(0x00);
        for (volatile int i = 0; i < 50000; i++);
		}
		
		//actualizo estados anteriores
		//last_state_b0 = b0;
    //last_state_b1 = b1;
		
		
		//LECTURA DEL RX (cuando recibe)
		
		uint8_t receiverDATA = receiver();
		
		switch(receiverDATA) {
        case 0x00:
            GPIOB_ODR &= ~((1 << 12) | (1 << 13)); // Ambos OFF
            break;
        case 0x01:
            GPIOB_ODR |=  (1 << 12);  // LED B12 ON
            GPIOB_ODR &= ~(1 << 13);  // LED B13 OFF
            break;
        case 0x02:
            GPIOB_ODR &= ~(1 << 12);  // LED B12 OFF
            GPIOB_ODR |=  (1 << 13);  // LED B13 ON
            break;
        case 0x03:
            GPIOB_ODR |=  (1 << 12);  // LED B12 ON
            GPIOB_ODR |=  (1 << 13);  // LED B13 ON
            break;
        default:
            break;
    }
		
		
	}
}


//TEST defuncionamiento de UART
/*int main(void){
    // Inicialización
    CLKconfig();
    USARTports();
    USARTconfig();

    PINconfig();
    while(1){
        // Transmitir 'A' y esperar su eco
        transmmitt('A');
        uint8_t r = receiver(); // espera el eco porque PA9->PA10 puenteado
        if (r == 'A'){
            // Enciende LED (ánodo al pin, cátodo a GND)
            GPIOB_ODR |= (1 << 12);
						GPIOB_ODR &= ~(1 << 13);
					
        } else {
            // Apagar si por alguna razón llega otro valor
            GPIOB_ODR &= ~(1 << 12);
						GPIOB_ODR |= (1 << 13);
        }
        // pequeño retardo
        for (volatile int i=0; i<200000; i++);
    }
}*/



