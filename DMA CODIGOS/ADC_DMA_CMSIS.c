/*
===============================================================================
 Name        : DMA_ADC_MEM_CMSIS.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc_types.h"
#include "lpc17xx_gpdma.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"

#endif

#include <cr_section_macros.h>

void ADC_Configuracion(void);
void DMA_Configuracion(void);


int main(void) {

	ADC_Configuracion();
	 DMA_Configuracion();


    while(1) {

    }
    return 0 ;
}

void ADC_Configuracion(void){
	//FALTA LA CONFIGURACION DE LOS PINES
	PINSEL_CFG_Type PinCfg;

		PinCfg.Portnum = 0;
				PinCfg.Pinnum = 23;
						PinCfg.Funcnum=	1;//PO[10] COMO TX
								PinCfg.OpenDrain= PINSEL_PINMODE_NORMAL;
										PinCfg.Pinmode=  PINSEL_PINMODE_TRISTATE;

										PINSEL_ConfigPin(&PinCfg);


	ADC_Init(LPC_ADC, 150000);//tasa de conversion
	ADC_ChannelCmd (LPC_ADC, 0, ENABLE);//HABILITO EL CANAL 0
	 ADC_PowerdownCmd(LPC_ADC, ENABLE);// HABILITA EL ADC EL PDN
	 ADC_BurstCmd(LPC_ADC, ENABLE	);// EN MODO RAFAGA O BURST

}

void DMA_Configuracion(void){

	GPDMA_LLI_Type DMA_LLI_Struct;//CREACION DE LA ESTRUCTURA LLY_TYPE
	GPDMA_Channel_CFG_Type  GPDMACfg;// creacion de la estructura chanel_CFG

	DMA_LLI_Struct.SrcAddr = (uint32_t)&(LPC_ADC->ADDR0);   ////ADC_ChannelGetData(LPC_ADC, 0);//uint16_t ancho resultado del ADC
	DMA_LLI_Struct.DstAddr = (uint32_t)0x10004000;// 16===16
	DMA_LLI_Struct.NextLLI = (uint32_t)&DMA_LLI_Struct;//forma de convalidar la estructura GPDMA_LLI_Type "ojo" no hay otra forma solo una lista de datos
	DMA_LLI_Struct.Control = 100 | (1<<21) | (1<<18) |(1<<27);
	GPDMA_Init();// ENCIENDE EL DMA Y limpia las banderas e inicializa

   GPDMACfg.ChannelNum =0;
   GPDMACfg.TransferSize =100;

   GPDMACfg.SrcMemAddr = ADC_ChannelGetData(LPC_ADC, 0);//se vuelve a repetir pero para la configuracion del canal
   GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_P2M;
   GPDMACfg.DstMemAddr = (uint16_t)0x10004000;
   GPDMACfg.TransferWidth = 16;
   GPDMACfg.DstConn =  0; //porque es a memoria
   GPDMACfg.SrcConn =  GPDMA_CONN_ADC;//segun la libreria de cmsis cuando es de periferico a memoria solo se configura la source
   GPDMACfg.DMALLI = (uint32_t)&DMA_LLI_Struct;//solo una lista de datos igual que  en la otra estructura lo vuelve a declarar

   GPDMA_Setup(&GPDMACfg);//VALIDA LA ESTRUCRUA DE CONFIGURACION DEL CANAL
    GPDMA_ChannelCmd(0,ENABLE);//"OJO IMPORTANTE" EL MANUAL RECOMIENDA HABILITAR EL CANAL DESPUES QUE SE HA CONFIGURADO EL TAMANIO
                               // DE LA TRANSFERENIA Y NO ANTES ES DECIR SE RECOMIENDA HABILITAR EL CANAL AL ULTIMO CUANDO
                               // SE HAYA CONFIGURADO AL ULTIMO










}






















