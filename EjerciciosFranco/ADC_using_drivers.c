#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"

#include "LPC17xx.h"

#define _ADC_INT		ADC_ADINTEN2
#define	_ADC_CHANNEL	ADC_CHANNEL_2

__IO uint32_t adc_value; //_IO makes the variable 'volatile'.
						 //This is to avoid optimization from the compiler
/*
 * The problem is that adc_value only changes through hardware, so the compile will see this
 * and store the variable close to the core, so it can be easier to get access to. The thing
 * is that, in order to see the change in the variable, we need to search it's value in the
 * original register in RAM.
 */

void configPin(void);
void configADC(void);

int main(void) {

	uint32_t tmp;

	configPin();
	configADC();

	while(1){
		//Start conversion
		ADC_StartCmd(LPC_ADC, ADC_START_NOW);

		//Enable ADC in NVIC
		NVIC_EnableIRQ(ADC_IRQn);

		for(tmp = 0; tmp < 1000000; tmp++); //retardo entre conversiones de ADC
	}

    return 0;
}


void configPin(){
	PINSEL_CFG_Type pincfg;
	/*
	 *	Initialize ADC pin connect
	 *	AD0.2 on P0.25
	 */
	pincfg.Funcnum		=	PINSEL_FUNC_1; //First alternate function, in this case ADC0.2 (PINSEL1, [18:19])
	pincfg.OpenDrain	=	PINSEL_PINMODE_NORMAL;
	pincfg.Pinmode		=	PINSEL_PINMODE_TRISTATE; //By request from the manufacturer
	pincfg.Pinnum		=	25;
	pincfg.Portnum		=	0;

	PINSEL_ConfigPin(&pincfg);

	return;
}

void configADC(void){
	/*
	 * Configuration for the ADC:
	 * 		Select:	ADC channel 2 (if using MCB1700 board)
	 * 				ADC channel 5 (if using IAR-LPC1768 board)
	 * 		ADC conversion rate = 200KHz
	 */

	ADC_Init(LPC_ADC, 200000); //set sampling rate in 200kHz
	ADC_IntConfig(LPC_ADC, _ADC_INT, ENABLE); //Enable ADC interrupts
	ADC_ChannelCmd(LPC_ADC, _ADC_CHANNEL, ENABLE);	//Enable channel 2

	NVIC_SetPriority(ADC_IRQn, (9));

	return;
}

void ADC_IRQHandler(void){
	adc_value = 0;
	if(ADC_ChannelGetStatus(LPC_ADC, _ADC_CHANNEL, ADC_DATA_DONE)){ //We need to ask if the channel 2 interrupted

		adc_value	=	ADC_ChannelGetData(LPC_ADC, _ADC_CHANNEL);	//Store digitalized data into adc_value

		NVIC_DisableIRQ(ADC_IRQn);	//Disable ints
	}


	return;
}








