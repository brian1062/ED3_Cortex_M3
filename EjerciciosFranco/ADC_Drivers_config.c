#include "LPC17xx.h"
#include "lpc17xx_adc.h"

void config_ADC(void);
void config_PIN(void);

int main(){
	config_PIN();
    config_ADC();

    return 0;
}

void config_ADC(){
    ADC_Init(LPC_ADC, 200000);
    ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE);
    ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_1, ENABLE);
    ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_2, ENABLE);
    ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_3, ENABLE);

    ADC_BurstCmd(LPC_ADC, ENABLE);

    ADC_StartCmd(LPC_ADC, ADC_START_NOW);

    return;
}

void config_PIN(){
	PINSEL_CFG_Type pincfg0, pincfg1, pincfg2, pincfg3;
	pincfg0.Portnum = PINSEL_PORT_0;
	pincfg0.Pinnum  = 23;
	pincfg0.Funcnum = 1;
	pincfg0.Pinmode = PINSEL_PINMODE_TRISTATE;
	pincfg0.OpenDrain = PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&pincfg0);

	pincfg1.Portnum = PINSEL_PORT_0;
	pincfg1.Pinnum  = 23;
	pincfg1.Funcnum = 1;
	pincfg1.Pinmode = PINSEL_PINMODE_TRISTATE;
	pincfg1.OpenDrain = PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&pincfg1);

	pincfg2.Portnum = PINSEL_PORT_0;
	pincfg2.Pinnum  = 23;
	pincfg2.Funcnum = 1;
	pincfg2.Pinmode = PINSEL_PINMODE_TRISTATE;
	pincfg2.OpenDrain = PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&pincfg2);

	pincfg3.Portnum = PINSEL_PORT_0;
	pincfg3.Pinnum  = 23;
	pincfg3.Funcnum = 1;
	pincfg3.Pinmode = PINSEL_PINMODE_TRISTATE;
	pincfg3.OpenDrain = PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&pincfg3);

	return;
}
