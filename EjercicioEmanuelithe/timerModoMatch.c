/*
===============================================================================
 Name        : timerModoMatch.c
 Author      : $Ema
 Version     :
 Copyright   :
 Description : main definition
===============================================================================
Utilizando el Timer 1, escribir un codigo en C para que por cada presion de un
pulsador, la frecuencia de parpadeo de un led disminuya a la mitad debido a la
modificacién del reloj que llega al periférico. El pulsador debe producir una
interrupcién por GPIOO con flanco descendente. Adjuntar el codigo en C.

*/

#include "LPC17xx.h"

void set_gpio(void);
void set_timer(void);
void set_int(void);

int valmatch;

int main(void) {
	set_gpio();
	set_int();
	set_timer();
    while(1) {}
    return 0 ;
}
void set_gpio(void){
	LPC_GPIO0->FIODIR &=~(1<<0); //P0 pin 0 como entrada para pulsador
	return;
}
void set_int(void){
	LPC_GPIOINT->IO0IntClr |=(1<<0);//limpio flags
	LPC_GPIOINT->IO0IntEnF |=(1<<0);//habilito int por flanco desc
	NVIC_EnableIRQ(EINT3_IRQn);
}
void set_timer(void){
	LPC_SC->PCONP |= (1<<2); //prendo timer1 aunque enrealidad ya esta prendido
	LPC_SC->PCLKSEL0|= (1<<4);//configuro prescaler de clock=cclk

	LPC_PINCON->PINSEL3 |=(3<<12);//configuro pin de salida por match

	LPC_TIM1->EMR |= (3<<4);//realiza un toggle cada vez que hace un match

	valmatch=5999999;//el match es cada 60ms
	LPC_TIM1->MR0 =valmatch;
	LPC_TIM1->MCR |=(1<<1);//hab reset cuando matchea
	LPC_TIM1->MCR &=~(1<<0);//deshabilito int cuando matchea

	LPC_TIM0->TCR =3; ////Iniciamos el timer y lo reseteamos
	LPC_TIM0->TCR &= ~(1 << 1); //sacamos cond de reset

}
void EINT3_IRQHandler(void){
	valmatch*=2;
	valmatch+=1;
	if (valmatch>48000005){
			valmatch=5999999;
		}
	LPC_TIM1->MR0 =valmatch;
	LPC_TIM1->TCR |= (1<<1);
	LPC_TIM1->TCR &= ~(1<<1); //QUITAMOS COND DE RESET
	LPC_GPIOINT->IO0IntClr |=(1<<0);//limpio flags
}
