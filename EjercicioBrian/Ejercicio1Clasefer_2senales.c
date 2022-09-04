/*
 * Brian Gerard
 * 23/08/2022
 * Encendido y apagado de un led en p0.22
 * */

#include "LPC17xx.h"

void retardo(void);

int main(void){

    //la pos 9 del P0 como entrada
    LPC_GPIO0->FIODIR &= ~(1<<9);
    //la pos 22 del P0 para usar el led rojo de la placa
    LPC_GPIO0->FIODIR |= (1<<22);
    //la pos 25 del P3 para usar el led verde
    LPC_GPIO3->FIODIR |= (1<<25);

    int arr_1[11]={0,1,1,0,1,0,1,0,0,1,0};
    int cont1=0;
    int arr_2[11]={0,0,0,0,0,0,0,0,0,1,1};
    int cont2=0;

    while(1){
        if((LPC_GPIO0->FIOPIN)&(1<<9)){ //prende led rojo
            cont2=0;
            LPC_GPIO3->FIOSET |= (1<<25);
            if(arr_1[cont1]==1){
                LPC_GPIO0->FIOSET |= (1<<22);
            }else{
                LPC_GPIO0->FIOCLR |= (1<<22);
            }
            cont1++;
            if(cont1==10){
                cont1=0;
            }
        }else{ //prende led verde
        	cont1=0;
        	LPC_GPIO0->FIOSET |= (1<<22);
        	if(arr_2[cont2]==1){
        	LPC_GPIO3->FIOSET |= (1<<25);
        	}else{
        	LPC_GPIO3->FIOCLR |= (1<<25);
        	}
        	cont2++;
        	if(cont2==10){
        	cont2=0;
        	}


        }
        retardo();
    }
    return 0;
}
void retardo(void){
    uint32_t contador;
    for(contador =0;contador<6000000;contador++);
}
