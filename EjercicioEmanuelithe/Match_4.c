/*
===============================================================================
 Name        : Match_4.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
Utilizando los 4 registros match del Timer 0 y las salidas P0.0, P0.1, P0.2 y P0.3,
realizar un programa en C que permita obtener las formas de ondas adjuntas, donde
los pulsos en alto tienen una duración de 5 mseg. Un pulsador conectado a la entrada
EINT3, permitirá elegir entre las dos secuencias mediante una rutina de servicio a
la interrupción. La prioridad de la interrupción del Timer tiene que ser mayor que
la del pulsador. Estas formas de ondas son muy útiles para controlar un motor paso
a paso. Adjuntar el código en C .

*/

#include "LPC17xx.h"

void set_gpio(void);
void set_int(void);
void set_timer(void);

int secuencia=0;

int main(void) {
	set_gpio();
	set_timer();
	set_int();

    while(1) {}
    return 0 ;
}

void set_gpio(void){
	LPC_PINCON->PINSEL0 &=~(0xFF<<0);//configuro desde P0.0 a P0.3 como GPIO
	LPC_GPIO0->FIODIR |= (0xF<<0); //Configuro desde P0.0 a P0.3 como salidas

	LPC_GPIO0->FIOPIN |=(1<<0);//seteo el primer valor prendido
}
void set_timer(void){
	LPC_SC->PCONP |=(1<<1);//habilito timer0
	LPC_SC->PCLKSEL0 |=(1<<2);//selecciono pclock/cclk

	//T=(1/pclk)*(PR+1)*(MR+1) = 5ms=(1/100x10^3)*(1)*(MR+1)
	LPC_TIM0->MR0 =499999; //5ms
	LPC_TIM0->MR1 =999999; //10ms
	LPC_TIM0->MR2 =1499999;//15ms
	LPC_TIM0->MR3 =1999999;//20ms

	LPC_TIM0->MCR |= ((1<<0)|(1<<3)|(1<<6)|(3<<9));
	//Habilito int cuando haga match en
	//M0 M1 M2 M3 y habilito reset en M3

	LPC_TIM0->TCR =3; ////Iniciamos el timer y lo reseteamos
	LPC_TIM0->TCR &= ~(1 << 1); //sacamos cond de reset

	LPC_TIM0->IR |= (0xF<<0);//limpio flag de timer

	NVIC_SetPriority(TIMER0_IRQn,3); //seteo prioridad de int
	NVIC_EnableIRQ(TIMER0_IRQn);//habilito int
}
void set_int(void){
	LPC_PINCON->PINSEL4 |= (1<<26); //selecciono modo eint3 P2.13
	LPC_SC->EXTMODE |=(1<<3); //pongo int habilitada por flanco
	LPC_SC->EXTPOLAR &=~(1<<3);//selecciono flanco descendente
	LPC_SC->EXTINT |= (1<<3); //limpio flag

	NVIC_SetPriority(EINT3_IRQn,4); //seteo prioridad de int
	NVIC_EnableIRQ(EINT3_IRQn);//habilito int
}
void EINT3_IRQHandler(void){
	if(secuencia==0){
		secuencia=1;
	}
	else{
		secuencia=0;
	}
	LPC_TIM0->TCR =3; ////Iniciamos el timer y lo reseteamos
	LPC_TIM0->TCR &= ~(1 << 1); //sacamos cond de reset
	LPC_GPIO0->FIOPIN |=(1<<0);//seteo el primer valor prendido
	LPC_SC->EXTINT |= (1<<3); //limpio flag interrupcion
}
void TIMER0_IRQHandler(void){
	if(secuencia==0){//secuencia 1
		if(LPC_TIM0->IR & (1)){//reviso match0
			LPC_GPIO0->FIOPIN |=(1<<1);//prendo salida pin1
			}
		else if(LPC_TIM0->IR & (1<<1)){//reviso match1
			LPC_GPIO0->FIOPIN |=(1<<2);//prendo salida pin2
		}
		else if(LPC_TIM0->IR & (1<<2)){//reviso match2
			LPC_GPIO0->FIOPIN |=(1<<3);//prendo salida pin3
		}
		else{//match3
			LPC_GPIO0->FIOPIN |=(1<<0);//prendo salida pin0
		}
	}
	else{//condicion de secuencia 2
		if(LPC_TIM0->IR & (1)){//reviso match0
			LPC_GPIO0->FIOPIN |=(3<<0);//prendo salida pin0 y pin1
			}
		else if(LPC_TIM0->IR & (1<<1)){//reviso match1
			LPC_GPIO0->FIOPIN |=(3<<1);//prendo salida pin1 y pin2
		}
		else if(LPC_TIM0->IR & (1<<2)){//reviso match2
			LPC_GPIO0->FIOPIN |=(3<<2);//prendo salida pin2 y pin3
		}
		else{//match3
			LPC_GPIO0->FIOPIN |=(9<<0);//prendo salida pin0 y pin3
		}
	}
	LPC_TIM0->IR |= (0xF<<0);//limpio flag de timer
}
