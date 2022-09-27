/*
===============================================================================
 Name        : lev_Timer_modo_Capture_1.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : Utilizando el modo Capture, escribir un código en C  para que
 guarde en una variable ,llamada "shooter", el tiempo (en milisegundos)
 que le lleva a una persona presionar dos pulsadores con un único dedo.
 Adjuntar  el archivo .c del código. Nombre de los archivos
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
int shooter;
// TODO: insert other include files here

void setTimer();

int main(void) {

	setTimer();
    while(1) {
    }
    return 0 ;
}
void setTimer(){
	LPC_SC->PCONP |= (1<<1);//TIMER0
	LPC_SC->PCLKSEL0   |=(1<<2);//CCLK SIN DIVIDIR

	LPC_PINCON->PINSEL3 |=(0xF<<20);//selecciono mod cap0.0 cap0.1 (pines p1.26/27)
	LPC_TIM0->CCR |= ((1)|(1<<3)|(1<<5));//POR RISING EDGE CAPTURA Y INTERRUMPE
	LPC_TIM0->IR |= (3<<4);//LIMPIO LAS FLAGS
	LPC_TIM0->TCR |= (3<<0);//LIMPIO CONTADOR Y RESETEO
	LPC_TIM0->TCR &= ~(1<<1);
	NVIC_EnableIRQ(TIMER0_IRQn);
}
void TIMER0_IRQHandler(){
	shooter= (LPC_TIM0->CR1-LPC_TIM0->CR0)/1000;//la diferencia /1000 para pasar a ms

	LPC_TIM0->TCR |= (3<<0);//LIMPIO CONTADOR Y RESETEO EL TIMER
	LPC_TIM0->TCR &= ~(1<<1);
	LPC_TIM0->IR |= (3<<4);//LIMPIO LAS FLAGS
}
