/*
===============================================================================
 Name        : DMA_DAC.c
 Author      : AYARDE
 Version     :
 Copyright   : $(copyright)
 Description : Se realiza una transferencia de una región de memoria
 	 	 	   hacia un DAC.
===============================================================================
*/

#include "lpc17xx_dac.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpdma.h"

#define DMA_SIZE 		60 //Es el tama;o de la region de memoria
#define NUM_SINE_SAMPLE 60 //Cantidad de datos en una onda senoidal que esta en la region de memoria
#define SINE_FREQ_IN_HZ 50 //Queremos que la onda senoidal salga por el DAC de tal forma que
						   //nos permita ver un ciclo completo con una frecuencia de 50Hz
/*Cuando el DAC se utiliza con DMA, hay que asociar (al DAC) un timer que marcara el ritmo
 * al cual se envian los datos desde la memoria al DAC	*/
#define PCLK_DAC_IN_MHZ 25 //CCLK divided by 4

void confPIN(void);
void confDMA(void);
void confDAC(void);

GPDMA_Channel_CFG_Type GPDMACfg; //Variable util para configurar el DMA
uint32_t dac_sine_lut[NUM_SINE_SAMPLE]; //Region de memoria en la cual cargaremos
										//los datos que queremos enviar al DAC

int main(void){

	uint32_t i;
	uint32_t sin_0_to_90_16_samples[16] = (\
			0, 1045, 2079, 3090, 4067, \
			5000, 5877, 6691, 7431, 8090, \
			8660, 9135, 9510, 9781, 9945, 10000\
			); //Estos 16 datos muestran la conformacion de un cuarto de onda senoidal

	confPIN();
	confDAC();

	//Prepare DAC sine look up table
	//En el for se arma la onda completa
	for(i = 0; i < NUM_SINE_SAMPLE; i++){
		if(i <= 15)
		{
			dac_sine_lut[i] = 512 + 512*sin_0_to_90_16_samples[i]/10000;
			if(i == 15) dac_sine_lut[i] = 1023;
		}
		else if(i <= 30)
		{
			dac_sine_lut[i] = 512 + 512*sin_0_to_90_16_samples[30-i]/10000;
		}
		else if(i <= 45){
			dac_sine_lut[i] = 512 - 512*sin_0_to_90_16_samples[i-30]/10000;
		}
		else
		{
			dac_sine_lut[i] = 512 - 512*sin_0_to_90_16_samples[60-i]/10000;
		}
		dac_sine_lut[i] = (dac_sine_lut[i] << 6);
	}

	confDMA();
	//Enable GPDMA channel 0
	GPDMA_ChannelCmd(0, ENABLE);

	while(1);


	return 0;
}

//El pin a configurar es el del DAC
void confPIN(void){
	PINSEL_CFG_Type PinCfg;

	/*
	 * Init DAC pin connect
	 * AOUT on P0.26
	 */
	PinCfg.Funcnum 		= 2;
	PinCfg.OpenDrain 	= 0;
	PinCfg.Pinmode 		= 0;
	PinCfg.Pinnum 		= 26;
	PinCfg.Portnum 		= 0;

	PINSEL_ConfigPin(&PinCfg);

	return;
}

void confDMA(void){
	GPDMA_LLI_Type DMA_LLI_Struct;

	//Prepare DMA Link list item structure
	DMA_LLI_Struct.SrcAddr = (uint32_t)dac_sine_lut;
	DMA_LLI_Struct.DstAddr = (uint32_t)&(LPC_DAC->DACR);
	DMA_LLI_Struct.NextLLI = (uint32_t)&DMA_LLI_Struct;
	DMA_LLI_Struct.Control = DMA_SIZE
			| (2<<18) //source width 32 bits
			| (2<<21) //dest. width 32 bits
			| (1<26)  //source increment
			;

	//GPDMA block section
	//Initialize GPDMA controller
	GPDMA_Init();

	//Setup GPDMA channel
	//channel 0
	GPDMACfg.ChannelNum 	= 0;
	//Source memory
	GPDMACfg.SrcMemAddr 	= (uint32_t)(dac_sine_lut);
	//Destination memory - unused
	GPDMACfg.DstMemAddr 	= 0;
	//Transfer size
	GPDMACfg.TransferSize 	= DMA_SIZE;
	//Transfer width - unused
	GPDMACfg.TransferWidth 	= 0;
	//Transfer type
	GPDMACfg.TransferType 	= GPDMA_TRANSFERTYPE_M2P; //memory 2 peripheral
	//Source connection - unused
	GPDMACfg.SrcConn 		= 0;
	//Destination connection
	GPDMACfg.DstConn		= GPDMA_CONN_DAC;
	//Linker List Item - unused
	GPDMACfg.DMALLI 		= (uint32_t)&DMA_LLI_Struct;

	//Setup channel with given parameter
	GPDMA_Setup(&GPDMACfg);

	return;
}

/* Hay que habilitar el uso del DMA y habilitar el uso del contador asociado al DAC
 *
 */
void confDAC(void){
	uint32_t tmp;
	DAC_CONVERTER_CFG_Type DAC_Cfg;

	DAC_Cfg.CNT_ENA		= SET;  //Este es el contador asociado
	DAC_Cfg.DMA_ENA		= SET;
	DAC_Init(LPC_DAC);			// Configuramos para que el DAC pueda soportar una
								// frecuencia de trabajo de 1MHz

	//Set time out for DAC
	tmp = (PCLK_DAC_IN_MHZ*1000000)/(SINE_FREQ_IN_HZ*NUM_SINE_SAMPLE);	// Se carga hasta que valor va a contar el timer asociado al DAC
																		// tal que cuando el timer llegue a ese valor, reinicie la cuenta
																		// y genere un disparo para que mediante el DMA se envie informacion
																		// desde la region de memoria a la saldia del DAC
	DAC_SetDMATimeOut(LPC_DAC, tmp);

	DAC_ConfigDAConverterControl(LPC_DAC, &DAC_Cfg);

	return;
}




// ~ 20 min clase 2





