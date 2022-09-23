/*
===============================================================================
 Name        : LEV_Calculo_tiempo_con_Timer.c
 Author      : $Brian
 Version     :
 Copyright   : $(copyright)
 Description :Utilizando el Timer 1, escribir un código en C
 para que por cada presión de un pulsador, la frecuencia de parpadeo
 de un led disminuya a la mitad debido a la modificación del reloj que
 llega al periférico. El pulsador debe producir una interrupción por GPIO0
 con flanco descendente. Adjuntar el código en C.
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif


#include <cr_section_macros.h>

void setTimer(void);
void setGpio(void);

int valmatch;
int cont=0;

int main(void) {
	setGpio();
	setTimer();

    return 0 ;
}

void setGpio(){
	LPC_GPIO0->FIODIR &= ~(1<<0);//seteamos el port0 . 0 com entrada
	LPC_GPIO0->FIOSET |= (1<<0); //Ponemos por defaul un 1
	//SETEAMOS INT
	LPC_GPIOINT->IO0IntClr |= (1<<0); // LIMPPIAMOS FLAG
	LPC_GPIOINT->IO0IntEnF |= (1<<0); //HABILITAMOS PARA INT POR FALLING
	NVIC_EnableIRQ(EINT3_IRQn);

}



void setTimer(){
	LPC_SC->PCONP |= (1<<2);//PARA HABILITAR TIMER1
	LPC_SC->PCLKSEL0 |= (1<<4);//SETEAMOS EN 0 EL N=1 ENTONCES PCLCK=CLCK

	LPC_PINCON->PINSEL3 |= (3<<12);//ACTIVO MAT1.0
	LPC_TIM1->EMR |= (3<<4);//realiza un toggle cada vez que hace un match
	valmatch=5999999;//para 60ms
	LPC_TIM1->MR0 = valmatch; // SETEO MATCH0
	LPC_TIM1->MCR |= (1<<1); // HABILITO RESET CUANDO MATCHEA0
	LPC_TIM1->MCR |= (1<<0); // HABILITO QUE INTERRUMPA CUANDO MATCHE

	LPC_TIM1->TCR |= 3;//HABILITAMOS cont tmr1 y reseteamos elcounter
	LPC_TIM1->TCR &= ~(1<<1); //QUITAMOS COND DE RESET
	//NVIC_EnableIRQ(TIMER1_IRQn);
};

void EINT3_IRQHandler(){
	if(cont==5){
		valmatch=5999999;
		cont=0;
	}else{
	valmatch*=2;
	cont++;
	}
	LPC_TIM1->MR0 = valmatch; // SETEO MATCH0

	LPC_TIM1->TCR |= (1<<1);//reseteamos el contador
	LPC_TIM1->TCR &= ~(1<<1); //QUITAMOS COND DE RESET

	LPC_GPIOINT->IO0IntClr |= (1<<0); // LIMPPIAMOS FLAG
}
