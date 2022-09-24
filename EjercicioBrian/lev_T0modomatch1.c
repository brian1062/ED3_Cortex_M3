/*
===============================================================================
 Name        : lev_timer_en_modo_Match_1.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : Escribir el código que configure el timer0 para
 cumplir con las especificaciones dadas en la figura adjunta.
 (Pag 510 Figura 115 del manual de usuario del LPC 1769).
 Considerar una frecuencia de cclk de 100 Mhz y una división de
 reloj de periférico de 2.
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here
void setTimer0();

int main(void) {
	setTimer0();
    while(1) {}
    return 0 ;
}

void setTimer0(){
	LPC_SC->PCONP |= (1<<1);//ACTIVO TIMER0

	LPC_SC->PCLKSEL0 |=(1<<3);//para dividir en 2 el reloj

	LPC_TIM0->PR = 2;
	LPC_TIM0->MR0 = 6;

	LPC_TIM0->MCR = 3;//cuando matchea interrupte y se resetea

	LPC_TIM0->TCR = 3;
	LPC_TIM0->TCR &= ~(1<<1);
	NVIC_EnableIRQ(TIMER0_IRQn);//habilito interrupcion
}

void TIMER0_IRQHandler(){

	//se hace q no especifica la consigna
	LPC_TIM0->IR |= (1<<0);

}
