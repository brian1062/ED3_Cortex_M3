/*
===============================================================================
 Name        : Dac_Fer.c
 Author      : $ema
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================

El ejercicio es poner 7 botones que cada uno genere las siguientes frecuencias:
B0 262 hz
B1 293 hz
B2 329 hz
B3 349 hz
B4 392 hz
B5 440 hz
B6 494 hz
*/

#include "LPC17xx.h"

void set_gpio(void);
void set_int(void);
void set_dac(void);
void set_timer(void);

int cont=0;
int sine[1500];

int main(void) {
	set_gpio();
	set_int();
	set_dac();
	set_timer();

    while(1) {}
    return 0 ;
}
void set_gpio(void){
	LPC_GPIO0->FIODIR &=~(0x7F<<0);//seteo P0.0 al P0.6 como entradas

}
void set_int(void){
	LPC_GPIOINT->IO0IntClr |= (0x7F<<0);//limpio flags
	LPC_GPIOINT->IO0IntEnF |= (0x7F<<0);//habilito int por flanco descendente P0.0 a P0.6
	NVIC_EnableIRQ(EINT3_IRQn);
}
void set_dac(void){
	LPC_PINCON->PINSEL1 |=(1<<21);//selecciono modo AOUT
	LPC_SC->PCLKSEL0 |= (1<<22); //cclk
	LPC_PINCON->PINMODE1 |= (3<<20);//open drain mode
	LPC_DAC->DACR |= (1<<16);//Bias en 1
}
void set_timer(void){//Por default arranca como si estuviera el B0
	//
	LPC_SC->PCONP |= (1<<1);//activo timer0
	LPC_SC->PCLKSEL0 |= (1<<2);//CCLK

	LPC_TIM0->MR0 = 381678;//para 262Hz
	LPC_TIM0->MCR |= (3<<0);//PARA Q INTERRUMPA Y SE REINICIE

	LPC_TIM0->TCR |= (3<<0);
	LPC_TIM0->TCR &= ~(1<<1);

	LPC_TIM0->IR |= (1<<0);//LIMPIO FLAG
	NVIC_EnableIRQ(TIMER0_IRQn);
}
void EINT3_IRQHandler(void){
	if(LPC_GPIOINT->IO0IntStatF & (1)){
		LPC_TIM0->MR0 = 381678;//para 262Hz
	}
	else if(LPC_GPIOINT->IO0IntStatF & (1<<1)){
		LPC_TIM0->MR0 = 341296;//para 293Hz
	}
	else if(LPC_GPIOINT->IO0IntStatF & (1<<2)){
		LPC_TIM0->MR0 = 303950;//para 329Hz
	}
	else if(LPC_GPIOINT->IO0IntStatF & (1<<3)){
		LPC_TIM0->MR0 = 286532;//para 349Hz
	}
	else if(LPC_GPIOINT->IO0IntStatF & (1<<4)){
		LPC_TIM0->MR0 = 255101;//para 392Hz
	}
	else if(LPC_GPIOINT->IO0IntStatF & (1<<5)){
		LPC_TIM0->MR0 = 227272;//para 440Hz
	}
	else{
		LPC_TIM0->MR0 = 202428;//para 494Hz
	}
	LPC_TIM0->TCR |= (3<<0);
	LPC_TIM0->TCR &= ~(1<<1);
	LPC_GPIOINT->IO0IntClr |= (0x7F<<0);//limpio flags
}
void TIMER0_IRQHandler(void) {
	uint32_t dacval;
	dacval = (LPC_DAC->DACR)&~(1023<<6);
	LPC_DAC->DACR = dacval|(sine[count]<<6);
	if(cont=1500){
		cont=0;
	}else{
		cont++;
	}
	LPC_TIM0->IR |= (1<<0);//LIMPIO FLAG
}
