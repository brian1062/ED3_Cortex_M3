#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_dac.h"

void confPin(void);
void confDAC(void);
void delay(void);


int main(void) {
	uint32_t dac_value[11] = {0, 123, 223, 323, 423, 523, 623, 723, 823, 923, 1023};
	confPin();
	confDAC();

	while(1){
		for(uint8_t count = 0; conta <11; count++){
			DAC_UpdateValue(LPC_DAC, dac_value[conta]);
			delay();
		}
	}

	return 0 ;
}


void confPin(void){
	//Setting pin P0.26 as AOUT
	PINSEL_CFG_Type pinselcfg;
	pinselcfg.Portnum	=	0;
	pinselcfg.Pinnum	=	26;
	pinselcfg.Funcnum	=	2;	//When 10, AOUT
	pinselcfg.Pinmode 	= 	0;	//PINSEL_PINMODE_PULLUP
	pinselcfg.OpenDrain	=	0;	//PINSEL_PINMODE_NORMAL

	PINSEL_ConfigPin(&pinselcfg);

	return;
}


void confDAC(void){
	DAC_Init(LPC_DAC);	//Sets DAC to run at max frequency, with 700uA (BIAS = 0)
	//In case 1MHz is not necessary, we can use the function DAC_SetBias with BIAS = 1
}

void delay(){
	//Delay should not be smaller than 1us, because BIAS = 0
	for(uint32_t i = 0; i<4000000; i++);

	return;
}

