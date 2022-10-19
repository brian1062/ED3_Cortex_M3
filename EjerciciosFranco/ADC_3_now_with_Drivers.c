/*
 * a.- Modificar el valor del Match del ejemplo visto en clases
 * para que la frecuencia de muestreo del ADC sea de 20 Kmuestras/seg.
 * El resultado de la conversión deben mostrarse por 12 pines de salida
 * del GPIO0.  Adjuntar el código en C.
 *
 * b.- Modificar el código realizado en el punto "a" utilizando ahora
 * el Driver provisto para este microcontrolador.
 */

#include "LPC17xx.h"
#include "lpc17xx_gpio.h"

void confGPIO(void); // Prototipo de la función de conf. de puertos
void confADC(void);  //Prototipo de la función de conf. de interrupciones externas
void confTimer(void);
void retardo(void);

int main(void) {
	confGPIO();
	confTimer();
	confADC();
	while(1){
		GPIO_SetValue(0, 22);
		retardo();
		GPIO_ClearValue(0, 22);
		retardo();
	}
	return 0;
}

void retardo (void){
	uint32_t conta;
	for(conta = 0; conta<1000000;conta++){}
	return;
}

void confGPIO(void){
	PINSEL_CFG_Type pincfg;
	pincfg.Portnum = PINSEL_PORT_0;
	pincfg.Pinnum  = PINSEL_PIN_23;
	pincfg.Funcnum = 1;
	pincfg.OpenDrain = PINSEL_PINMODE_NORMAL;
	pincfg.Pinmode = PINSEL_PINMODE_TRISTATE;
	PINSEL_ConfigPin(&pincfg);


	GPIO_SetDir(0, 22, 1);
	GPIO_SetDir(0, 0, 1);
	GPIO_SetDir(0, 1, 1);
	GPIO_SetDir(0, 2, 1);
	GPIO_SetDir(0, 3, 1);
	GPIO_SetDir(0, 4, 1);
	GPIO_SetDir(0, 5, 1);
	GPIO_SetDir(0, 5, 1);
	GPIO_SetDir(0, 7, 1);
	GPIO_SetDir(0, 8, 1);
	GPIO_SetDir(0, 9, 1);
	GPIO_SetDir(0, 10, 1);
	GPIO_SetDir(0, 11, 1);

	return;
}

void confADC(void){
	ADC_Init(LPC_ADC, 20000);
	ADC_BurstCmd(LPC_ADC, DISABLE);
	ADC_StartCmd(LPC_StartCmd, ADC_START_ON_MAT01);
	ADC_EdgeStartConfig(LPC_ADC, FALLING);
	ADC_IntConfig(LPC_ADC, ADC_ADINTEN0, Enable);

	NVIC_EnableIRQ(ADC_IRQn);

	return;
}

void confTimer(void){
	TIM_TIMERCFG_Type prescale;
	prescale.PrescaleOption 	= TIM_PRESCALE_USVAL;
	prescale.PrescaleValue 		= 1;

	TIM_MATCHCFG_Type match;
	match.ResetOnMatch 			= ENABLE;
	match.ExtMatchOutputType 	= TIM_EXTMATCH_TOGGLE;
	match.MatchValue			= 25;
	match.MatchChannel 			= 1;
	match.IntOnMatch 			= DISABLE;
	match.StopOnMatch 			= DISABLE;

	TIM_SetMatchExt(LPC_TIM0, TIM_EXTMATCH_TOGGLE);

	TIM_Init(LPC_TIM1,  TIM, TIM_TIMER_MODE, &prescale);
	TIM_ConfigMatch(LPC_TIM1, &match);

	TIM_Cmd(LPC_TIM1, ENABLE); //Habilitamos el TMR1;

	return;
}


void ADC_IRQHandler(void)
{
	//static uint16_t ADC0Value = 0;
	//float volt = 0;
	LPC_GPIO0->FIOPIN = ((LPC_ADC->ADDR0)>>4) & 0xFFF; //Variable auxiliar para observar el valor del registro de captura
	//if(ADC0Value>2055)
	//	LPC_GPIO0->FIOSET |= (1<<9);
	//else
	//	LPC_GPIO0->FIOCLR |= (1<<9);
	//volt=(ADC0Value/4096)*3.3;
	return;
}

