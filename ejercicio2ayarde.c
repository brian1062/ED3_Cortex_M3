/*
 * Brian Gerard
 * 23/08/2022
 * Encendido y apagado de un led en p0.22
 * */

#include "LPC17xx.h"

void retardo(void);

int main(void){
	//se configura el bit 12 y 13
	LPC_GPIO2->FIODIR |= (0b11111111);//00111111111111 &
	//la pos 22 es la
	uint8_t numDisplay [10] ={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};
	int contador=0;
	while(1){
		LPC_GPIO2->FIOCLR0 |= 0xFF;
		LPC_GPIO2->FIOSET0 |= numDisplay[contador];
		retardo();
		contador++;
		if(contador==10){contador=0;}
	}
	return 0;
}
void retardo(void){
	uint32_t contador;
	for(contador =0;contador<6000000;contador++);
}
