/*
===============================================================================
 Name        : capture_1.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
Utilizando el modo Capture, escribir un código en C  para que guarde en una
variable ,llamada "shooter", el tiempo (en milisegundos) que le lleva a una
persona presionar dos pulsadores con un único dedo. Adjuntar  el archivo .c
del código. Nombre de los archivos
*/

#include "LPC17xx.h"
int shooter;
void set_timer(void);

int main(void) {
	set_timer();
    while(1) {}
    return 0 ;
}

void set_timer(void){
	LPC_SC->PCONP |=(1<<1);//habilito timer0
	LPC_SC->PCLKSEL0 |=(1<<2); //selecciono pclock/cclk

	LPC_PINCON->PINSEL3 |= (0xF<<20);//selecciono modo cap0.0 y cap0.1 en los pines
	//P1.26 y p1.27
	LPC_TIM0->CCR |= ((1<<0)|(1<<3)|(1<<5));//cap por flanco de subida e int en el cap1

	LPC_TIM0->TCR =3; ////Iniciamos el timer y lo reseteamos
	LPC_TIM0->TCR &= ~(1 << 1); //sacamos cond de reset

	LPC_TIM0->IR |= (3<<4);//limpio flag de timer
	NVIC_EnableIRQ(TIMER0_IRQn);//habilito int
}

void TIMER0_IRQHandler(void){
	shooter= ((LPC_TIM0->CR1)-(LPC_TIM0->CR1))/1000;//resultado en milisegundos

	LPC_TIM0->TCR =3; ////Iniciamos el timer y lo reseteamos
	LPC_TIM0->TCR &= ~(1 << 1); //sacamos cond de reset
	LPC_TIM0->IR |= (3<<4);//limpio flag de timer
}
