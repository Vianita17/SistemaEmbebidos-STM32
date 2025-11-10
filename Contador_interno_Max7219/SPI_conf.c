
//----SPI_config
#include "stm32f10x.h"
#include <stdint.h>

// ----------- Registros usados 
#define RCC_APB2ENR (*(volatile uint32_t*) 0x40021018) //registro encendido perifericos
#define GPIOA_CRL   (*(volatile uint32_t*) 0x40010800)	// registros pines 0-7
#define GPIOA_ODR   (*(volatile uint32_t*) 0x4001080C)	// registro de escritura pines
#define GPIOA_BSRR  (*(volatile uint32_t*) 0x40010810)	// registros set/reset
#define GPIOA_IDR   (*(volatile uint32_t*) 0x40010808)	// registros de lectura de pines

#define SPI1_CR1    (*(volatile uint32_t*) 0x40013000) //registro configuracion spi
#define SPI1_SR     (*(volatile uint32_t*) 0x40013008) // regsitro para configuracion de las banderas
#define SPI1_DR     (*(volatile uint16_t*)0x4001300C) //registro de datos

//uso de CS
static inline void CS_LOW(void)  { GPIOA_BSRR = (1 << (16 + 4)); } // usamos como bandera para reinciar el pin4 load -> 0
static inline void CS_HIGH(void) { GPIOA_BSRR = (1 << 4); } //load -> 1


void conf_spi(void); //prototipo de todo la configuracion
void spi1_tx(uint8_t data); //prototipo de captura de datos
void Transmite(uint8_t reg, uint8_t dato); //enviar datos al max
void zerosLEDS(void);

void conf_spi(void) {
    // 1) Habilitar reloj GPIOA y SPI1
    RCC_APB2ENR |= (1 << 2);   // GPIOA
    RCC_APB2ENR |= (1 << 12);  // SPI1
		RCC_APB2ENR |= (1 << 0);

    // 2) Configurar PA4 (CS), PA5 (SCK), PA7 (MOSI)
    GPIOA_CRL &= ~((0xF << (4*4)) | (0xF << (4*5)) | (0xF << (4*7)));
    GPIOA_CRL |=  (2 << (4*4));   // PA4 = Salida push-pull
    GPIOA_CRL |=  (0xB << (4*5)); // PA5 = AF push-pull
    GPIOA_CRL |=  (0xB << (4*7)); // PA7 = AF push-pull

    // 3) Configurar SPI1: master, BR=PCLK/16, 8 bits, SSM/SSI habilitados
    SPI1_CR1 &= ~(1 << 6);       // SPE = 0 (deshabilitar SPI para configurar)
    SPI1_CR1 &= ~(7 << 3);       // limpiar BR
    SPI1_CR1 |=  (3 << 3);       // BR=011 - fPCLK/16
    SPI1_CR1 |=  (1 << 2);       // MSTR=1 (master)
    SPI1_CR1 |=  (1 << 9);       // SSM=1
    SPI1_CR1 |=  (1 << 8);       // SSI=1
    SPI1_CR1 |=  (1 << 6);       // SPE=1 (habilitar SPI)

    // inicializar MAX7219
    Transmite(0x0C, 1);    // Shutdown ? normal operation
    Transmite(0x09, 0xFF); // Decode mode BCD todos los dígitos
    Transmite(0x0B, 7);    // Scan limit ? 8 dígitos
    Transmite(0x0A, 8);    // Intensidad ? media
		Transmite(0x0F, 0);    // Display test ? off
		
		
		//vamos apagar todos los led
		zerosLEDS();
}
void zerosLEDS(){ //funcion para apagar los led
	for (int i=1; i < 9; i++){
			Transmite( i, 0x00);    // display para que todos se pongan en ceros]
			//Transmite( i , 0x0F);    // display para que esten apagados
		}
}
	
// ----------- Envio de un byte por SPI 
void spi1_tx(uint8_t data) {
    while (!(SPI1_SR & (1<<1)));  // espera TXE=1
    SPI1_DR = data;  //asigno el dato al registro que quiero usar
}

// ----------- Envio de (registro, dato) al MAX7219 
void Transmite(uint8_t reg, uint8_t dato) {
    CS_LOW();
    spi1_tx(reg); //mando el registro 
    spi1_tx(dato); //mando  el dato
    while (SPI1_SR & (1<<7)); // espera BSY=0
    CS_HIGH();
}
