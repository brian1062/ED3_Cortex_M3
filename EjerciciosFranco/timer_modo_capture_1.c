/*
===============================================================================
 Name        : timer_modo_capture_1.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : Utilizando el modo Capture, escribir un código en C  para que
 	 	 	   guarde en una variable ,llamada "shooter", el tiempo (en milisegundos)
 	 	 	   que le lleva a una persona presionar dos pulsadores con un único dedo.
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <stdbool.h>

#include <lpc17xx_timer.h>

void config_timer();

int shooter 		= 0;
int first_time_in 	= 1;
int first_capture 	= 0;


int main(void) {
	config_timer();
	while(1);

    return 0 ;
}

void config_timer(){

	/*Configuramos el prescaler para que desborde cada 1ms. Es decir, el
	 * timer aumentara en cada milisegundo. */
	TIM_TIMERCFG_Type prescale;
	prescale.PrescaleOption 	= TIM_PRESCALE_USVAL;
	prescale.PrescaleValue	 	= 1000;

	TIM_CAPTURECFG_Type capture;
	capture.CaptureChannel		= 0;
	capture.FallingEdge			= ENABLE;
	capture.RisingEdge			= DISABLE;
	capture.IntOnCaption		= ENABLE;

	TIM_Init(LPC_TIM0, TIM_COUNTER_FALLING_MODE, &prescale);
	TIM_ConfigCapture(LPC_TIM0, &capture);

	TIM_Cmd(LPC_TIM0, ENABLE); 	//Habilitamos el TMR0

	NVIC_EnableIRQ(TIMER0_IRQn);


	return;
}

int TIMER0_IRQHandler(void){

	if(first_time_in){
		first_time 		= TIM_GetCaptureValue(LPC_TIM0, 0);
	}else{
		shooter 		= TIM_GetCaptureValue(LPC_TIM0, 0);
		TIM_ResetCounter(LPC_TIM0);
		first_time_in 	= 0;
		LPC_TIM0->TCR 	&= ~(1<<1);
		LPC_TIM0->IR 	|= (3<<4);	//LIMPIO LAS FLAGS
		return shooter-first_time;
	}

	first_time_in		 = 0;
	LPC_TIM0->TCR 		&= ~(1<<1);
	LPC_TIM0->IR 		|= (3<<4);	//LIMPIO LAS FLAGS

	return first_time;
}
