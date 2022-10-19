#include "LPC17xx.h"

#define PIN_22 ((uint32_t)(1<<22))

void confGPIO(void);
void confADC(void);

uint16_t ADC0Value = 0;

int main(void){

	confGPIO();
	confADC();
	while(1);


	return 0;
}


void confGPIO(void){
	LPC_GPIO0->FIODIR |= PIN_22;

	return;
}


void confADC(void){
	LPC_SC->PCONP 	  |= 	(1<<12); //El bit en la posicion 12 del registro PCON
									 //es el responsable de encender el ADC
	LPC_ADC->ADCR	  |= 	(1<<21); //Habiliatamos el ADC. Es distinto prender el periferico
									 //de habilitarlo. En el caso del ADC, es necesario
									 //prenderlo, habilitarlo, deshabilitarlo, apagarlo, en ese orden.
	LPC_SC->PCLKSEL0  |=	(3<<24); //En las posiciones 24 y 25 se encuentran los bits
									 //que controlan el clock que entra el ADC. En este caso,
									 //CCLK/8
	LPC_ADC->ADCR	  &=   ~(0xFF<<8); //[8:15] CLKDIV. Esta es la frecuencia de trabajo final del ADC
	//LPC_ADC->ADCR		|=	(1<<0); //channel
	LPC_ADC->ADCR	  |=	(1<<16);//Se utilizara en modo burst
	LPC_PINCON->PINMODE1 =  (1<<15); //neither pull-down nor pull-up are enabled
	LPC_PINCON->PINSEL1  =  (1<<14); //Para mi tendria que ser (1<<15), si no estaria en repeater mode
	LPC_ADC->ADINTEN   = 	1; //completion of a conversion on ADC channel 1 will generate an interrupt

	NVIC_EnableIRQ(ADC_IRQn); //Habilitamos las interrupciones a traves del NVIC

	return;
}


void ADC_IRQHandler(void){
	ADC0Value = ((LPC_ADC->ADDR0)>>4) & 0xFFF; //Variable auxiliar para
											   //observar el valor de la conversion
	//La multiplicacion por 0xFFF es una mascara para obtener solo los 12 valores de la conversion

	float valor_en_volts;
	//valor_en_volts = ADC0Value*3.3/4095;
	valor_en_volts = (ADC0Value/4095)*3.3; //Dividir y luego multiplicar le cuesta menos al compilador
	//Hay que tener cuidado porque la placa no soporta el punto flotante. Es decir,
	//el HW no es capaz de manejarlo pero si el SW, por lo que el unico problema que
	//representaria es la cantidad de ciclos que se emplean manejando el punto flotante

	if(ADC0Value < 2054){
		LPC_GPIO0->FIOSET = PIN_22;
	}else{
		LPC_GPIO0->FIOCLR = PIN_22;
	}

	return;
}
