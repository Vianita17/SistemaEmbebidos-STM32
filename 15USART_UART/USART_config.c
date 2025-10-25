
//author: Julian Adrian Viana Palomo
#include "stm32f10x.h"                  // Device header
#include "address.h"	//direcciones de memoria
#include "stdint.h" //C

/*
	CCodigo para configurar el modulo de uso de USART para protocolo de comunicacion UART
	Funciones:
			transmmit -> trasmitir los datos desde DR
			receiver -> recibir los datos desde el pint Tx de otro dispositivo
			USARTconfig -> configurar USART como UART
			
	conexion 
	pin9 (TX) - pin10(Rx)
	pin10(Rx) - pin9(Tx)		
*/


/*
void USARTports(void);
void USARTconfig(void);
void transmmitt(void);
void receiver(void);*/

//configuracion pines 9 & 10
void USARTports(void){
	
	//RCC_APB2ENR &= ~((1<<0)|(1<<2));//limpiamos bits del registro
	//RCC_APB2ENR |= (1<<0); //encendemos la funcion alternativa
	//RCC_APB2ENR |= (1<<2); //encendemos puerto A	
	
	GPIOA_CRH &= ~(0xF << 4); //limpiamos bits para pin9 & pin10
	GPIOA_CRH |= (0xB<<4); //pin9 -> output push/pull 50Mhz
	
	GPIOA_CRH &= ~(0xF << 8);
	GPIOA_CRH |= (0xB<<8); //pin10 -> Input Floating
}


//	USARTconfig -> configurar USART como UART
void USARTconfig(void){
	
	
	//RCC_APB2ENR &= ~(1<<14); //limpiamos bit del USART1
	//RCC_APB2ENR |= (1<<14); //habilitamos USART1
	
	//USARTports();//configuramos los pines9 Tx y pin10 Rx
	
	USART1_CR1 &= ~(1<<13); //deshabilitamos USART para configurar
	/*
	parametros a configurar
		baud rate = 115200
		word leght = 8
		parity = none
		stop bit = 1
	*/
	
	
	USART1_CR1 = 0; //limpiamos registro entero
	//para asegurar en ceros
	
	//baud rate -> fclk/(16xUSARTDIV)
	//115200 = 8Mhz/ (16xUSARTDIV)
	//USARTDIV = 8Mhz/(115200*16)
	//USARTDIV = 625/144 = 4.34 
	
	//DIV_Fraction 0:3 = .340277*16= 5.444 = 0b0101
	//DIV_Mantissa 4:15 = 4 = 0b0100
	
	USART1_BRR = (4<<4) | 5;

	//con lo anterior aseguramos baud rate de 115200
	
	USART1_CR1 |= (0<<12); //word leght -> 8bits
	USART1_CR1 |= (0<<10); //PEN -> disabled -> decir que no queremos control de paridad
												//parity: none
	
	USART1_CR2 = 0; //limpiamos registro entero
	USART1_CR2 |= (0b00 << 12); //STOP BIT -> 1 -> 00
	
	//habilitamos USART1
	USART1_CR1 = (1<<13) | (1<<3) | (1<<2); // UE, TE, RE habilitados
}


//funcion para transmitir datos
void transmmitt(uint8_t value){ //transmitir valores para configuracion de 8 bits
		while(!(USART1_SR & (1<<7))); // espeta TXE
    USART1_DR = value; //guardo el valor a transmitir a DR
    while(!(USART1_SR & (1<<6))); // TC
	
}


//funcion de detectar cuando reciba un dato y guardarlo
uint8_t receiver(void){
    //while(!(USART1_SR & (1<<5))); // espero RXNE
		if (USART1_SR & (1<<5)){
			return (uint8_t)USART1_DR; //retornos lo que se guardo en DR
		}else{	
			return 0xFF; //retorno lo que se guardo en DR
		}
}


