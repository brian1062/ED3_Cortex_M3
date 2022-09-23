/*
===============================================================================
 Name        : calculotiempocontimer.c
 Author      : $ema
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================

Calcular cual es el tiempo maximo que se puede temporizar utilizando un timer
en modo match con maximo valor de prescaler y maximo divisor de frecuencia de
periférico. Especificar el valor a cargar en los correspondientes registros del
timer. Suponer una frecuencia de core cclk de 100 Mhz.

*/


//Calculo
/*
 * T=(1/pclk)*(PR+1)*(MR+1)
 * donde pclk=cclk/n, en este caso como quiero el maximo n es 8
 * PR= 2^32
 * MR= 2^32
 * T=1,47x10^12 segundos
 */
