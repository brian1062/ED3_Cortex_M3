/*
===============================================================================
 Name        : ADC_triggeredby_TIMER.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "LPC17xx.h"

void configGPIO(void);
void confADC(void);
void confTimer(void);
void retardo(void);

int main(void) {

	confGPIO();
	confTimer();
	confADC();

	while(1){
		LPC_GPIO0->FIOSET 	|= (1<<22);
		retardo();
		LPC_GPIO0->FIOCLR	|= (1<<22);
		retardo();
	}

    return 0;
}


void retardo(void){
	uint32_t count;
	for(count = 0; count < 1000000; count++);

	return;
}


void confGPIO(void){
	LPC_GPIO0->FIODIR |= (1<<22)|(1<<9); //Set P0.9 and P0.22 as outputs

	return;
}


void confADC(void){
	LPC_SC->PCONP	|= 	(1<<12); //Turn on ADC
	LPC_ADC->ADCR	|=	(1<<22); //Enable ADC
	LPC_SC->PCLKSEL0|=	(3<<24); //CCLK/8
	LPC_ADC->ADCR	&= ~(255<<8);//CLKDIV[15:8] = 1
	//LPC_ADC->ADCR	|=	(1<<0);	 //channel
	LPC_ADC->ADCR	&= ~(1<<16); //burst is disabled, conversions are
								 //software controlled and require 65 clocks
	LPC_ADC->ADCR	|= 	(1<<26); //match
	LPC_ADC->ADCR	&= ~(3<<24); //match
	//bits [24:26] helps us select the start mode. When 100: "start conversion when
	//the edge selected by bit 27 occurs on MAT0.1. Note that this does not require
	//that the MAT0.1 function appear on a device pin
	LPC_ADC->ADCR	|=	(1<<27); //start conversion on a falling edge on the selected CAP/MAT signal
	//LPC_PINCON->PINSEL4	|=	(1<<20); //not necessary according to user manual
	LPC_PINCON->PINMODE1	|=	(1<<15); //neither pull-up nor pull-down
	LPC_PINCON->PINSEL1		|= 	(1<<14); //input is no analogical

	LPC_ADC->ADINTEN		|=1;
	NVIC_EnableIRQ(ADC_IRQn);

	return;
}


void confTimer(void){
	LPC_SC->PCONP	|=	(1<<1); //By default, timers 0 and 1 are always on
	LPC_SC->PCLKSEL0|=	(1<<2);	//Peripheral clock configuration
	//LPC_PINCON->PINSEL3	|=	(3<<24); Mode MAT0.0... shouldn't it be MAT0.1? It should
	LPC_TIM0->EMR	|= 	(3<<6);//External Match Register. When 11, toggles is enabled
							   //When a match occurs between TC and MR1, the bit toggles
	LPC_TIM0->MR1	 =	100000000; //Match value will be 100e6. So... every time the timer
								   //counts up to 100e6, triggers the ADC
	LPC_TIM0->MCR	|=	(1<<4); //Reset on match enabled
	LPC_TIM0->TCR	|=	(3<<0); //Timer Control Register. When bit 0 is 1, timer counter and prescale counter are enabled for counting.
								//when bit 1 is 1, timer counter and prescale counter are synchronously reset on the next positive edge of PCLK.
	LPC_TIM0->TCR	&= ~(1<<1); //The counters remain reset until TCR[1] is returned to zero, n thats what we doin in this line

	//The timer generates a falling edge every 2 seconds. Meaning that the ADC
	//is triggered every 2 seconds

	//Thus, sampling frequency 0.5Hz

	return;
}

void ADC_IRQHandler(void){
	static uint16_t ADC0Value = 0;
	float volt = 0;
	ADC0Value = ((LPC_ADC->ADDR0)>>4) & 0xFF; //Aux variable to store the ADC result

	if(ADC0Value>2055){
		LPC_GPIO0->FIOSET	|=	(1<<9);
	}else{
		LPC_GPIO0->FIOCLR	|= 	(1<<9);
	}

	volt = (ADC0Value/4096)*3.3;

	return;
}
