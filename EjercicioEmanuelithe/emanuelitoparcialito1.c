/*
===============================================================================
 Name        : EjercicioParcialFer.c
 Author      : $Ema
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================

Produzca una interrupción por flanco descendente en EINT2 sacar por pin P2.4 la secuencia
. En caso que se produzca una nueva interrupción por EINT2 mientras se está realizando la
secuencia , se pondrá en uno la salida P2.4 y se dará por finalizada la secuencia . El programa
NO debe hacer uso de retardos por software y deben enmascararse los pines del puerto 2
que no van a ser utilizados . Suponer una frecuencia de reloj cclk de 60Mhz . Se pide el
programa completo debidamente comentado y los respectivos cálculos de tiempo
asignados al Systick .

*/
#include "LPC17xx.h"

void Secuencia (void);
//void EINT2_IRQHandler(void);
//void SysTick_Handler(void);
void SetSysTick(void);

uint8_t estado= 0;//variable para saber en que estado me encuentro
uint8_t arr[10]={0,0,0,0,1,0,1,0,1,0};//arreglo que contiene la secuencia de salida
uint8_t cont1= 0;//variable para moverme por el contador

int main(void) {
	//LPC_GPIO2->FIODIR  |= (1<<4); //seteo P2.4 como salida
	LPC_GPIO0->FIODIR |= (1<<22);//seteo como salida led rojo de la placa

	LPC_PINCON->PINSEL4 |= (1<<24);//seteo gpio EINT2
	LPC_GPIO2->FIODIR1 &= ~(1<<4); //P2.12 como entrada

	LPC_SC->EXTMODE |= (1<<2); //Selecciona interrupcion por flanco
	LPC_SC->EXTPOLAR &= ~(1<<2); //Interrumpe cuando el flanco es de bajada.
	LPC_SC->EXTINT|= (1<<2);//Limpio las banderas

	NVIC_SetPriority(EINT2_IRQn,1);//Seteo prioridad 1 para EINT2
	NVIC_EnableIRQ(EINT2_IRQn);//habilito interrupcion externa 2

	LPC_GPIO2->FIOMASK &= ~(1<<4);//enmascaro todo el puerto2 menos el P2.4

	NVIC_SetPriority(SysTick_IRQn,2);//seteo prioridad 2 a la interrupcion de systick

	while(1){}
	return 0;
}

void Secuencia (void){//secuencia de salida
	if(arr[cont1]==0){
		//LPC_GPIO2->FIOSET |= (1<<4);
		LPC_GPIO0->FIOSET |= (1<<22);
	}
	else{
		//LPC_GPIO2->FIOCLR |= (1<<4);
		LPC_GPIO0->FIOCLR |= (1<<22);
	}
	cont1++;
	if(cont1==10){
		cont1=0;
	}
}

void EINT2_IRQHandler(void) {
	if(estado==0){
		estado=1;
		Secuencia();
		SetSysTick();
	}
	else if(estado==1){
		//LPC_GPIO2->FIOSET |= (1<<4);
		estado=2;
		SysTick->CTRL =0;//apaga systick
		LPC_GPIO0->FIOCLR |= (1<<22);//prende led rojo
	}
	else if(estado==2){
		//LPC_GPIO0->FIOCLR |= (1<<4);
		LPC_GPIO0->FIOSET |= (1<<22);//apaga led rojo
		estado=0;
	}
	LPC_SC->EXTINT |= (1<<2);//limpia el flag de la interrupcion
}
void SysTick_Handler(void){
	Secuencia();
	SysTick->CTRL &= SysTick->CTRL;//reinicia systick
}

void SetSysTick(void){
	SysTick->CTRL=0;
	SysTick->LOAD=5999999;//((6M*10ms)-1)
	SysTick->VAL=0;
	SysTick->CTRL=0x00000007;
}
