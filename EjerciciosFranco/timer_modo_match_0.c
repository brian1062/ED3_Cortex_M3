/*
===============================================================================
 Name        : timer_modo_match_0.c
 Author      : $Franco
 Copyright   : $(copyright)
 Description : Utilizando el Timer 1, escribir un código en C para que por cada
 	 	 	   presión de un pulsador, la frecuencia de parpadeo de un led
 	 	 	   disminuya a la mitad debido a la modificación del reloj que llega
 	 	 	   al periférico. El pulsador debe producir una interrupción por
 	 	 	   GPIO0 con flanco descendente. Adjuntar el código en C.
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"

#include <cr_section_macros.h>

#define OUTPUT 	(uint8_t) 1
#define INPUT 	(uint8_t) 0

#define PORT_ZERO (uint8_t)		0
#define PORT_ONE (uint8_t) 		1
#define PORT_TWO (uint8_t) 		2
#define PORT_THREE (uint8_t) 	3

void config_GPIO(void);
void config_Timer1(int);

int match_value = 1000;

int main(void) {

	config_GPIO();
	config_Timer1(match_value);

	while(1){};


    return 0;
}



void config_GPIO(void){

	//Seteamos el PIN0.0 como input, con resistencia de pull-up activada
	//en modo GPIO sin open-drain
	PINSEL_CFG_Type push_button;
	push_button.Portnum 	= 	PORT_ZERO;
	push_button.Pinnum 		= 	PINSEL_PIN_0;
	push_button.Funcnum 	= 	PINSEL_FUNC_0;
	push_button.Pinmode 	= 	PINSEL_PINMODE_PULLUP;
	push_button.OpenDrain 	= 	PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&push_button);


    GPIO_SetDir(PORT_ZERO, PINSEL_PIN_0, INPUT);

	//Estableceremos al PIN0.0 capaz de interrupir por flanco de bajada.
	GPIO_IntCmd(PORT_ZERO, PINSEL_PIN_O, 1);

	//Habilitamos las interrupciones por EINT3 a traves del NVIC
	NVIC_EnableIRQ(EINT3_IRQn);


	return;
}

void EINT3_IRQHandler(){
	if(match_value > 8000){
		match_value = 1000;
	}else{match_value *= 2;}

	TIM_Cmd(LPC_TIM1, DISABLE); //Deshabilitamos el TMR1
	TIM_ResetCounter(LPC_TIM1); //Reseteamos el TMR1
	TIM_UpdateMatchValue(LPC_TIM1, 0, match_value);//Actualizamos el valor del match value
	TIM_Cmd(LPC_TIM1, ENABLE); //Habilitamos el TMR1

	LPC_GPIOINT->IO0IntClr |= (1<<0); //Limpiamos el flag de interrupcion

	return;
}

void config_Timer1(int match_value){
	TIM_TIMERCFG_Type prescale_mode;
	prescale_mode.PrescaleOption 	= TIM_PRESCALE_USVAL; 	//Lo que carguemos en PrescaleValue
													//sera el valor en el que se aumente PrescaleCounter
	prescale_mode.PrescaleValue 	= 1000;

	TIM_MATCHCFG_Type match_mode;
	match_mode.ResetOnMatch 		= ENABLE;
	match_mode.ExtMatchOutputType 	= TIM_EXTMATCH_TOGGLE;
	match_mode.MatchValue			= match_value;//Inicialmente, buscamos que matchee cada 1s
										//Suponemos CLK = 100 y prescaler = 8
	match_mode.MatchChannel			= 0;
	match_mode.StopOnMatch			= DISABLE;
	match_mode.IntOnMatch			= DISABLE;

	TIM_Init(LPC_TIM1,TIM_TIMER_MODE, &prescale_mode);
	TIM_ConfigMatch(LPC_TIM1, &match_mode);

	TIM_Cmd(LPC_TIM1, ENABLE); //Habilitamos el TMR1

	return;
}


