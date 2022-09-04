/*
===============================================================================
 Name        : EjercicioAyarde1.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "LPC17xx.h"

void retardo(int);
void changeFrec(int,int);

int main(void) {
	//seteamos como salida 2 led
	LPC_GPIO0->FIODIR |= (1<<22);//ROJO
	LPC_GPIO3->FIODIR |= (1<<26);//AZUL
	//seteo una entrada
	LPC_GPIO0->FIODIR &= ~(1<<9);

	int frec=0;
	int delay;
	LPC_GPIO3->FIOSET |= (1<<26);
	int cont=20;

	while(1){
		if((LPC_GPIO0->FIOPIN) & (1<<9)){
			delay=1000000;
			changeFrec(frec,delay);
		}else{
			delay=500000;
			changeFrec(frec,delay);
		}
		cont--;
		if(cont==0){
			if(frec==0){
				frec=1;
			}else{frec=0;}
			cont=20;
		}
	}
    return 0 ;
}
void retardo(int frec){
    uint32_t contador;
    for(contador =0;contador<frec;contador++);
}
void changeFrec(int frec,int delay){
	if(frec==0){
		LPC_GPIO0->FIOCLR |= (1<<22);
		retardo(delay);
		LPC_GPIO0->FIOSET |= (1<<22);
		retardo(delay);
		LPC_GPIO3->FIOCLR |= (1<<26);
		retardo(delay);
		LPC_GPIO3->FIOSET |= (1<<26);
		retardo(delay);
	}else{
		LPC_GPIO0->FIOCLR |= (1<<22);
		retardo(delay);
		retardo(delay);
		LPC_GPIO0->FIOSET |= (1<<22);
		retardo(delay);
		LPC_GPIO3->FIOCLR |= (1<<26);
		retardo(delay);
		retardo(delay);
		retardo(delay);
		LPC_GPIO3->FIOSET |= (1<<26);
		retardo(delay);
	}
}
