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
		LPC_GPIO0->FIOSET = (1<<22);
		retardo();
		LPC_GPIO0->FIOCLR = (1<<22);
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
	LPC_GPIO0->FIODIR |= (1<<22);//|(1<<9);
	LPC_GPIO0->FIODIR |= (0xFFF);

	return;
}

void confADC(void){
	LPC_SC->PCONP |= (1 << 12);
	LPC_ADC->ADCR |= (1 << 21);   //enable ADC
	LPC_SC->PCLKSEL0 |= (3<<24);  //CCLK/8
	LPC_ADC->ADCR |= ~(255<< 8);  //[15:8] CLKDIV
	//LPC_ADC->ADCR |= (1 << 0);   // channel
	LPC_ADC->ADCR &= ~(1 << 16);   // no burst
	LPC_ADC->ADCR |= (1 << 26);    // match
	LPC_ADC->ADCR &= ~(3 << 24);   // match
	LPC_ADC->ADCR |= (1 << 27);    //edge
	//LPC_PINCON->PINSEL4 |= (1<<20);
	LPC_PINCON->PINMODE1 |= (1<<17); //neither pull-up nor pull-down.
	LPC_PINCON->PINSEL1 |= (1<<16);
	LPC_ADC->ADINTEN |= (1<<0);
	NVIC_EnableIRQ(ADC_IRQn);

	return;
}

void confTimer(void){
	LPC_SC->PCONP |= (1 << 1); //Por defecto timer 0 y 1 estan siempre prendidos
	LPC_SC->PCLKSEL0 |= (1<<2); // configuración del clock del periférico
	//LPC_PINCON->PINSEL3 |= (3<<24);
	LPC_TIM0->EMR|=(3<<6); //toggle
	LPC_TIM0->MR1 = 2499;
	LPC_TIM0->MCR = (1<<4); //reset on match
	LPC_TIM0->TCR = 3;      // hab y reset
	LPC_TIM0->TCR &= ~(1<<1);
	//LPC_TIM0->PR = (0:2**32);

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

