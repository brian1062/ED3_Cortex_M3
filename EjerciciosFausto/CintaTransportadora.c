/*
===============================================================================
 Name        : Pre-parcial2.c
 Author      : Fausto Lavezzari
 Description : Parcial 2019. Cinta transportadora.
===============================================================================
*/


#include "LPC17xx.h"



uint8_t *inferior = (uint8_t *)0x10003005;
uint8_t *superior = (uint8_t *)0x10003001;
uint8_t datos[10];
uint8_t mediciones = 0;

int main(void) {
	LPC_PINCON->PINSEL4 |= 1<<20;
	LPC_PINCON->PINSEL4 &= ~(1<<21);
	LPC_SC->EXTINT |= 1;
	LPC_SC->EXTMODE |= 1;
	LPC_SC->EXTPOLAR |= 1;

	LPC_PINCON->PINSEL1 &= ~(3<<8);
	LPC_GPIO0->FIODIR |= (1<<20);

	LPC_PINCON->PINSEL1 &= ~(3<<10);
	LPC_GPIO0->FIODIR |= (1<<21);

	LPC_PINCON->PINSEL0 &= ~(0xF);
	LPC_GPIO0->FIODIR0 &= ~(0XF);

	SysTick->CTRL = 0;
	SysTick->LOAD = 400e-6*SystemCoreClock -1;
	SysTick->VAL = 0;

	NVIC_EnableIRQ(EINT0_IRQn);

	LPC_GPIO0->FIOPIN |= 1<<20;

	while(1){}

	return 0;
}

void EINT0_IRQHandler(void){
	LPC_GPIO0->FIOPIN &= ~(1<<20);
	SysTick->CTRL = 7;
	LPC_SC->EXTINT |= 1;
}

void SysTick_Handler(void){
	if(mediciones<10){
		datos[mediciones] = LPC_GPIO0->FIOPIN0;
		mediciones++;
	}

	if(mediciones == 10){
		uint8_t promedio = 0;
		for(uint8_t i = 0; i<10; i++){
			promedio += datos[i];
		}
		promedio /= 10;

		if(promedio < *superior && promedio > *inferior){
			LPC_GPIO0->FIOPIN |= 1<<20;
		}
		else{
			LPC_GPIO0->FIOPIN |= 1<<21;
		}
		SysTick->CTRL = 0;
		mediciones = 0;
	}
	SysTick->CTRL &= SysTick->CTRL;
}
