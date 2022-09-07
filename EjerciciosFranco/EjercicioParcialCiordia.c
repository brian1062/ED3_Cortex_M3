/*
===============================================================================
 Name        : ParcialCiordia.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 Utilizando Systick, programar al microcontrolador para que cada
 vez que se produzca una interrupción por flanco ascendente en el
 pin P2.4 se saque por el pin P2.5 la secuencia mostrada en la figura.
 Secuencia: [000010101010000]
 En caso que se produzca una nueva interrupción por P2.4 mientras se
 está realizando la secuencia , ésta volverá a realizarse desde su
 inicio descartando la que se venia sacando por el pin . El programa
 NO debe hacer uso de retardos por software y deben enmascararse los
 pines del puerto 2 que no van a ser utilizados. Suponer una frecuencia
 de reloj cclk de 80 Mhz ( NO se pide configuración del reloj ). Se pide
 el programa completo debidamente comentado y los respectivos cálculos de
 tiempo asignados al Systick.
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

void setGPIO(void);
void setInt(void);
void setSystick(void);

uint8_t sequence[] = {0,0,0,0,1,0,1,0,1,0,1,0,0,0,0};
uint8_t flag = 1;

int main(void) {

    //Asumo que los pines de estan, por defecto, seteados
	//como GPIO con resistencias de pull-up habilitadas.
	setGPIO();
	setInt();
	setSystick();


	while(1);

	return 0;
}

void setGPIO(void){
	LPC_GPIO2->FIOMASK |= ~(0b11<<4); //Enmascaro todos los pines menos P2.4 y P2.5
	LPC_GPIO2->FIODIR  |= (1<<5); //Establezco al P2.5 como salida
	LPC_GPIO2->FIODIR  &= ~(1<<4); //Establezco al P2.4 como entrada
}

void setInt(void){
	LPC_GPIOINT->IO2IntClr |= (1<<4); //Limpiamos el flag
	LPC_GPIOINT->IO2IntEnR |= (1<<4); //Habilitamos las interrupciones por flanco ascendente a traves del pin P2.4
	NVIC_EnableIRQ(EINT3_IRQn);//Habilitamos las interrupciones globales para EINT3
	NVIC_SetPriority(EINT3_IRQn, 5);

	return;
}


void EINT3_Handler(void){

	LPC_GPIOINT->IO2IntClr = 0; //Limpiamos el flag de interrupcion para que se pueda volver a interupir

	for(int i = 0; i< sizeof(sequence); i++){
		if(sequence[i] == 1){
			LPC_GPIO2->FIOPIN |= (1<<5);
		}else{
			LPC_GPIO2->FIOPIN &= ~(1<<5);
		}
		flag = 1;
		setSystick();
		while(flag);
	}

	return;
}

void setSystick(){
	SysTick->CTRL = 0;
	SysTick->VAL = 0;
	SysTick->LOAD = 7999;//(cclock*ms_wanted-1)
	SysTick->CTRL = 0x7;
	NVIC_SetPriority(SysTick_IRQn, 3);
}

void SysTick_Handler(void){

	flag = 0;
	SysTick->CTRL &= SysTick->CTRL; //Limpiamos el flag de interrupcion
}











