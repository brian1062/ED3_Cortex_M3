/*
===============================================================================
 Name        : EJDac_teorico.c
 Author      : $ema
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/
#include "LPC17xx.h"

void configDac();
void setTimer();
uint32_t dacvalue[2]={0,1023};
int cont=0;

int main(void) {
	configDac();
	setTimer();
    while(1) {}
    return 0 ;
}

void configDac(){
	LPC_PINCON->PINSEL1 |= (1<<21);//CONF AOUT P0.26
	LPC_SC->PCLKSEL0 |= (1<<22);//CCLK
	LPC_PINCON->PINMODE1 |= (1<<21);//OPEN drain mode
	LPC_DAC->DACR |= (1<<16);//BIAS EN 1

}
void setTimer(){//5micros = 200khz
	LPC_SC->PCONP |= (1<<1);//activo timer0
	LPC_SC->PCLKSEL0 |= (1<<2);//CCLK
	//MATCH 499
	LPC_TIM0->MR0 = 499;
	LPC_TIM0->MCR |= (3<<0);//PARA Q INTERRUMPA Y SE REINICIE

	LPC_TIM0->TCR |= (3<<0);
	LPC_TIM0->TCR &= ~(1<<1);

	LPC_TIM0->IR |= (1<<0);//LIMPIO FLAG
	NVIC_EnableIRQ(TIMER0_IRQn);
}

void TIMER0_IRQHandler(){
	uint32_t dacval;
	dacval = (LPC_DAC->DACR)&~(1023<<6);
	LPC_DAC->DACR = dacval|(dacvalue[count]<<6);
	if(cont=0){
		cont++;
	}else{
		cont=0;
	}
	LPC_TIM0->IR |= (1<<0);//LIMPIO FLAG
}
