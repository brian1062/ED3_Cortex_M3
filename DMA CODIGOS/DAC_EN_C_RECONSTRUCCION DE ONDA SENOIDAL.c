//DAC_EN_C_RECONSTRUCCION DE ONDA SENOIDAL
/*
===============================================================================
 Name        : DAC_sinusoide_joel.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

//-- DEFINE
#define NUM_SAMPLE_SINE         60
//-- COSNTANTES
uint32_t sin_0_to_90_16_samples[16]={\
                         	 	 	 0,1045,2079,3090,4067,\
									 5000,5877,6691,7431,8090,\
									 8660,9135,9510,9781,9945,10000\
									};
uint32_t dac_lut[NUM_SAMPLE_SINE];


//-- Config TMR0
void config_TMR0( void ){
	LPC_SC->PCON |= (1<<1); // Habilito T0 desde el Source Control
	LPC_TIM0->CTCR = 0; 	 // Timer0 en modo Temporizador

	LPC_TIM0->PC   = 25 ;  // cada que PC cuente 25mil TC incrementa en 1
	LPC_TIM0->PR   = 0 ;  // Preescaler, cuenta hasta éste valor para incrementar TMRO contador

	//-- Match:
	LPC_TIM0->MR0  = 250; // 250000; Este valor no se resetea automaticamente
	LPC_TIM0->MR1  = 500; //  500000; El valor mayor es el que debe resetearse en la config

	LPC_TIM0->MCR |= (1<<0);  // Interrupt on MR0[0]
	LPC_TIM0->MCR &= ~(1<<1); // No reset on MR0[0]
	LPC_TIM0->MCR |= (1<<3);  // Interrupt on MR0[1]
	LPC_TIM0->MCR |= (1<<4);  // Reset on MR0[1]
    










	//--Interrupciones en NVIC
	LPC_TIM0->TCR |= (1<<0); //  timer counter y prescaler counter habilitado
	LPC_TIM0->TCR |= (1<<1); //  se resete el timer counter y cuando sincroniza con el prescaler counter
	LPC_TIM0->TCR &=~ (1<<1); // Reseteamos el Contador

	LPC_TIM0->IR  &=~ (3<<0); // Bajamos la bandera de MR0[0] y MR0[1]
	LPC_TIM0->IR  &=~ (1<<1); // Bajamos la bandera de MR0[0] y MR0[1]

}

//-- Config NVIC:
void config_NVIC(void){
	NVIC_EnableIRQ(TIMER0_IRQn);
}
//-- Config DAC:
void config_DAC(void){

	//-- Config Clock:
	LPC_SC->PCLKSEL0 &= ~(1<<22);
	LPC_SC->PCLKSEL0 &= ~(1<<23); // PCLKdac = 100/4 = 25Mhz

	//-- Config Pin:
	LPC_PINCON->PINSEL1 &= ~(1<<20); //
	LPC_PINCON->PINSEL1 |= (1<<21);  // P0[26] Function '10'= AOUT, Salida del DAC
	//LPC_PINCON->PINMODE1 &=~ (1<<20);
	//LPC_PINCON->PINMODE1 |= (1<<21); // P0[26] Pin Mode '10'= Sin Pull-UP/Down

}


//-- TIMER0 IRQHandler
void TIMER0_IRQHandler(void){
	static int step=0;
	int value_dac;
	if(step<=15)
	{
		value_dac = 512 + 512*sin_0_to_90_16_samples[step]/10000;
		if(step==15) value_dac= 1023;
	}
	else if(step<=30)
	{
		value_dac = 512 + 512*sin_0_to_90_16_samples[30-step]/10000;
	}
	else if(step<=45)
	{
		value_dac = 512 - 512*sin_0_to_90_16_samples[step-30]/10000;
	}
	else
	{
		value_dac = 512 - 512*sin_0_to_90_16_samples[60-step]/10000;
	}
	LPC_DAC->DACR = (value_dac<<6);
	step++;
	step=step%60;

	if( LPC_TIM0->IR & (1<<0) ){
		LPC_TIM0->IR |= (1<<0); 	// Bajo Bandera MR0[0]
	}else if ( LPC_TIM0->IR & (1<<1) ){
		LPC_TIM0->IR |= (1<<1); 	// Bajo Bandera MR0[1]
	}
}

/****************************************
 *------ MAIN
****************************************/
int main(void) {
	config_TMR0();
	config_DAC();
	config_NVIC();
    while(1) {
    }
    return 0 ;
}