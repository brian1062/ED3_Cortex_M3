#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_dac.h"

void confPin(void);
void confDAC(void);
void delay(void);

int main(){
	uint32_t dac_value[11]={0,123,223,323,423,523,623,723,823,923,1023};
	confPin();
	confDAC();
	
	while(1){
		for(uint8_t conta = 0;conta<11;conta++){
			DAC_UpdateValue(LPC_DAC,dac_value[conta]);
			delay();
		}
	}
	return 0;
}
void confPin(void){
	//Configuración pin P0.26 como AOUT
	PINSEL_CFG_Type pinsel_cfg;
	pinsel_cfg.Portnum = 0;
	pinsel_cfg.Pinnum = 26;
	pinsel_cfg.Funcnum = 2;
	pinsel_cfg.Pinmode = 0;
	pinsel_cfg.OpenDrain = 0;
	PINSEL_ConfigPin(&pinsel_cfg);
	return;
}
void confDAC(void){
	DAC_Init (LPC_DAC);
}
void delay(){
	for (uint32_t i=0;i<4000000;i++){}
	return;
}
