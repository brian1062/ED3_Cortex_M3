/*
===============================================================================
 Name        : lev_Utilizando_mdo_Match_4.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : Utilizando los 4 registros match del Timer 0 y
 las salidas P0.0, P0.1, P0.2 y P0.3, realizar un programa en C
 que permita obtener las formas de ondas adjuntas, donde los
 pulsos en alto tienen una duración de 5 mseg. Un pulsador
 conectado a la entrada EINT3, permitirá elegir entre las dos
 secuencias mediante una rutina de servicio a la interrupción.
 La prioridad de la interrupción del Timer tiene que ser mayor
 que la del pulsador. Estas formas de ondas son muy útiles para
 controlar un motor paso a paso. Adjuntar el código en C .
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
int flag=0;
void setGpio();
void setTimer0();
void setEINT3();

int main(void) {

	setGpio();
	setTimer0();
	setEINT3();
    while(1) {
    }
    return 0 ;
}

void setGpio(){
	LPC_PINCON->PINSEL0 &= ~(0xff<<0);//modo gpio para p0.0/4
	LPC_GPIO0->FIODIR |= (0xF<<0);//los seteo como salidas
	LPC_GPIO0->FIOPIN  |= (1<<0); //Seteamos el primer valor
}
void setTimer0(){
	//match0 5ms,10ms,15ms,20ms
	LPC_SC->PCONP |= (1<<1);//Hab tim0
	LPC_SC->PCLKSEL0 |= (1<<2);//Utilizo el cclk sin dividir
	//(1/cclk)*(mtch+1)=5ms -> como cclk=100mhz
	LPC_TIM0->MR0 = 499999;
	LPC_TIM0->MR1 = 999999;
	LPC_TIM0->MR2 = 1499999;
	LPC_TIM0->MR3 = 1999999;

	LPC_TIM0->MCR = ((1<<0)|(1<<3)|(1<<6)|(3<<9));
	LPC_TIM0->TCR |= (3<<0);//RESEO EL CONT DEL TMR0 Y LO HABILITO
	LPC_TIM0->TCR &= ~(1<<1);
	LPC_TIM0->IR |=(0xF<<0);//LIMPIO FLAG DE IR por match
	NVIC_SetPriority(TIMER0_IRQn,3);
	NVIC_EnableIRQ(TIMER0_IRQn);
}
void setEINT3(){
	LPC_PINCON->PINSEL4 |= (1<<26);//habilitamos modo EINT3
	LPC_SC->EXTMODE |= (1<<3);//POR EDGE
	LPC_SC->EXTPOLAR &= ~(1<<3);//POR FALLING EDGE
	LPC_SC->EXTINT |= (1<<3);//LIMPIO FLAG
	NVIC_SetPriority(EINT3_IRQn,4);
	NVIC_EnableIRQ(EINT3_IRQn);
}
void EINT3_IRQHandler(){
	if(flag==0){
		flag=1;
	}else{
		flag=0;}
	LPC_TIM0->TCR |= (3<<0);//RESEO EL CONT DEL TMR0 Y LO HABILITO
	LPC_TIM0->TCR &= ~(1<<1);
	LPC_GPIO0->FIOPIN  |= (1<<0); //Seteamos el primer valor
	LPC_SC->EXTINT |= (1<<3);//LIMPIO FLAG EINT3
}
void TIMER0_IRQHandler(){
	if(flag==0){
		if(LPC_TIM0->IR&(1)){//1er match
			LPC_GPIO0->FIOPIN  |= (1<<1);
		}else if(LPC_TIM0->IR&(1<<1)){//2do match
			LPC_GPIO0->FIOPIN  |= (1<<2);
		}else if(LPC_TIM0->IR&(1<<2)){//3er match
			LPC_GPIO0->FIOPIN  |= (1<<3);
		}else{//4to match
			LPC_GPIO0->FIOPIN  |= (1<<0);
		}
	}else{
		if(LPC_TIM0->IR&(1)){//1er match
			LPC_GPIO0->FIOPIN  |= (3<<0);
		}else if(LPC_TIM0->IR&(1<<1)){//2do match
			LPC_GPIO0->FIOPIN  |= (3<<1);
		}else if(LPC_TIM0->IR&(1<<2)){//3er match
			LPC_GPIO0->FIOPIN  |= (3<<2);
		}else if(LPC_TIM0->IR&(1<<3)){//4to match
			LPC_GPIO0->FIOPIN  |= (1<<3);
			LPC_GPIO0->FIOSET  |= (1<<0);
		}
	}
	LPC_TIM0->IR |=(0xF<<0);//LIMPIO FLAG DE IR por match
}
