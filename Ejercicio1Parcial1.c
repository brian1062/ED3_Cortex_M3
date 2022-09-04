/*
===============================================================================
 Name        : Ejercicio1Parcial1.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description :
 produzca una interrupción por flanco descendente en EINT2 sacar por pin P2.4 la secuencia .
 En caso que se produzca una nueva interrupción por EINT2 mientras se está realizando la secuencia,
 se pondrá en uno la salida P2.4 y se dará por finalizada la secuencia.
 El programa NO debe hacer uso de retardos por software y deben enmascararse los pines del puerto 2 que no van a ser utilizados.
 Suponer una frecuencia de reloj cclk de 60Mhz .
 Se pide el programa completo debidamente comentado y los respectivos cálculos de tiempo asignados al Systick.
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>


uint8_t flag=0;
uint8_t indice=0;
uint8_t Salida[10] ={0,0,0,0,1,0,1,0,1,0};

void setGPIO(void);
void setIntExt(void);
void setSystick(void);
void SetSalida(void);
//void EINT2_IRQHandler(void);
//void SysTick_Handler(void);


int main(void) {
	setGPIO();
	setIntExt();
	LPC_GPIO2->FIOMASK &= ~(1<<4); //enmascaro los pines que no utilizo

	while(1){};
    return 0 ;
}
void setGPIO(){
	LPC_GPIO0->FIODIR |= (1<<22); //SETEAMOS COMO SALIDA P2.4
	LPC_GPIO0->FIOSET |= (1<<22); //mandamos 0 inicialmente a la salida
}

void setSystick(void){
	SysTick->CTRL = 0;
	SysTick->LOAD =  ((SystemCoreClock/100)-1);//599999;//(60E6*10E-3)-1=599999
	SysTick->VAL= 0;
	SysTick->CTRL=0x00000007;//HABILITO INTERRUPCION
	//NVIC_EnableIRQ(SysTick_IRQn); //NO ES NECESARIO
	NVIC_SetPriority (SysTick_IRQn, (NVIC_GetPriority(EINT2_IRQn)+1) );
}

void setIntExt(){
	LPC_PINCON->PINSEL4 |= (1<<24);//activo INT2
	//LPC_PINCON->PINMODE4 &= ~(3<<24); // Entrada con pullUp
	LPC_GPIO2->FIODIR1 &= ~(1<<4); //P2.12 como entrada


	LPC_SC->EXTINT |= (1<<2); //limpio flag
	LPC_SC->EXTMODE |= (1<<2); //Activo int por flanco
	LPC_SC->EXTPOLAR &= ~(1<<2);//modifico para flanco descendente
	NVIC_EnableIRQ(EINT2_IRQn);//habilito interrupcion EINT2
	NVIC_SetPriority (EINT2_IRQn, 0 );
}

void SetSalida(void){
	if(Salida[indice]==0){
		LPC_GPIO0->FIOSET |= (1<<22);
	}else{
		LPC_GPIO0->FIOCLR |= (1<<22);
	}
	indice++;
	if(indice==10){
		indice=0;
	}
}
void EINT2_IRQHandler(void){
	if(flag==0){
		flag=1;
		setSystick();//seteas y activas systick
		//SysTick->CTRL |= (1<<1); //Habilita la int Systick
	}else if(flag==1){
		flag=2;
		SysTick->CTRL = 0;//desabilito systick
		LPC_GPIO0->FIOCLR |= (1<<22); //mandamos 1 a la salida
	}
	LPC_SC->EXTINT |= (1<<2); //limpio flag
}

void SysTick_Handler(void) {
	SetSalida();
	SysTick->CTRL &= SysTick->CTRL;//
}
