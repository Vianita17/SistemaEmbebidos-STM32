
# Controlando TIM1 STM32 con un PUSH-BUTTON

Este codigo emplea el manejo de un modulo MAX7219 7 segementos atraves de la STM32F103C8

Codigo divido en 4 archivos, separados en :
* SPI_conf.c: Configuracion de la comunicacion SPI en los puertos A7, A4, A5
* clk_config.c: Configuracion del uso de reloj (osciladores)
* push_button_timer.c : Configuracion del TIM1 para funcionar y detectar los cambios del canal 2 en el pin A9



##  Diagrama de conexion
> Se puede poner una resistencia en pull-up para evitar por software. Sin embargo en el codigo se activa la que tiene cada uno de los pines

<p align="center">
<img src="https://github.com/Vianita17/SistemaEmbebidos-STM32/blob/main/Imagenes/Diagrama%20MaX-STM32.png?raw=true" alt="diagram" width="700">
</p>

## Referencias
* Maxim Integrated. (2002). MAX7219/MAX7221: Serially interfaced, 8-digit LED display drivers (Rev. 2002). Maxim Integrated Products. https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf
* STMicroelectronics. (2007). STM32F10xxx8/B/C/D/E: Medium-density performance line ARM®-based 32-bit MCU (UM0306, Rev. 7). STMicroelectronics. https://www.st.com/resource/en/reference_manual/cd00246267.pdf
## Compilador usado
* keil uVision4 5.43


## Creditos 
* Julian Adrian Viana Palomo
* Facultad de matematicas UADY 2025
