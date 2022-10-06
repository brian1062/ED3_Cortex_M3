/*
===============================================================================
 Name        : timer_en_modo_match.c
 Author      : $Franco
 Version     :
 Copyright   : $(copyright)
 Description : Escribir el código que configure el timer0 para cumplir con las
 	 	 	   especificaciones dadas en la figura adjunta. (Pag 510 Figura 115
 	 	 	   del manual de usuario del LPC 1769). Considerar una frecuencia
 	 	 	   de cclk de 100 Mhz y una división de reloj de periférico de 2.
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"

#define OUTPUT 	(uint8_t) 1
#define INPUT 	(uint8_t) 0

#define PORT_ZERO (uint8_t)		0
#define PORT_ONE (uint8_t) 		1
#define PORT_TWO (uint8_t) 		2
#define PORT_THREE (uint8_t) 	3

#include <cr_section_macros.h>

void config_timer0(void);


int main(void) {

    config_timer0();
    while(1);

}



void config_timer0(void){
	//PC must be 2
	//Match must be 6
	//Timer must rest and interrupt on match

	TIM_TIMERCFG_Type prescale_mode;
	prescale_mode.PrescaleOption 	= TIM_PRESCALE_USVAL;
	prescale_mode.PrescaleValue 	= 1000000;

	TIM_MATCHCFG_Type match_mode;
	match_mode.MatchChannel			= 0;
	match_mode.IntOnMatch			= ENABLE;
	match_mode.ResetOnMatch			= ENABLE;
	match_mode.ExtMatchOutputType 	= TIM_EXTMATCH_NOTHING;
	match_mode.StopOnMatch			= DISABLE;
	match_mode.MatchValue			= 6;

	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &prescale_mode);
	TIM_ConfigMatch(LPC_TIM0, &match_mode);

	TIM_Cmd(LPC_TIM0, ENABLE); //Habilitamos el TMR0

	NVIC_EnableIRQ(TIMER0_IRQn);

	return;
}
