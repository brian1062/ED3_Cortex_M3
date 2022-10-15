/*
===============================================================================
 Name        : configuracion_ADC_2.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : Configurar 4 canales del ADC para que funcionando en modo burst
 	 	 	   se obtenga una frecuencia de muestreo en cada uno de 50Kmuestras/seg.
 	 	 	   Suponer un cclk = 100 Mhz
===============================================================================
*/

#include "LPC17xx.h"


void config_ADC(void);


int main(void) {
	config_ADC();

    return 0;
}

void config_ADC(){
	LPC_SC->PCONP	|=	(1<<12); //Encendemos el ADC a traves del registro PCONP
	LPC_ADC->ADCR	|=	(1<<21); //Habilitamos el ADC a traves del bit PDN

	LPC_SC->PCLKSEL0|=	(3<<24); 	//Seleccionamos division del CCLK/8 para el ADC
	LPC_ADC->ADCR 	&=	(0x00<<8); 	// Esta es la frecuencia final de trabajo que le llega al ADC
								// Ffinal = CCLK/PCLKSEL0/(AD0CR+1)
									// En nuestro caso, Ffinal = 100Mhz/8/(0+1)


	LPC_PINCON->PINSEL1  	|=	(1<<14); //Los pines P0.23 P0.24 P0.25 P0.26 seran entradas al ADC
	LPC_PINCON->PINMODE1 	|= 	(1<<15); //Resistencias pull-up pull-down, ambas deshabilitadas

	LPC_ADC->ADCR	|=	(1<<16); //Habilitamos el modo burst

	LPC_ADC->ADCR	|=	(0xF); //Habilitamos AD0, AD1, AD2 y AD3

	return;
}











