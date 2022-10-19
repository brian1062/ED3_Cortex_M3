#include "LPC17xx.h"
#include "lpc17xx_gpdma.h"
#define DMA_SIZE 16
//DMAScr_Buffer will be burn into flash when compile
const uint32_t DMASrc_Buffer[DMA_SIZE]=
{
		0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
		0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
		0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
		0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40
};
uint32_t DMADest_Buffer[DMA_SIZE];
// Terminal Counter flag for Channel 0
volatile uint32_t Channel0_TC;
// Error Counter flag for Channel 0
volatile uint32_t Channel0_Err;
void DMA_IRQHandler (void);
void Buffer_Verify(void);
void confDMA(void);
int main(void)
{
	confDMA();
	/* Reset terminal counter */
	Channel0_TC = 0;
	/* Reset Error counter */
	Channel0_Err = 0;
	// Enable GPDMA channel 0
	GPDMA_ChannelCmd(0, ENABLE);
	/* Enable GPDMA interrupt */
	NVIC_EnableIRQ(DMA_IRQn);
	/* Wait for GPDMA processing complete */
	while ((Channel0_TC == 0) && (Channel0_Err == 0));
	/* Verify buffer */
	Buffer_Verify();
	/* Loop forever */
	while(1);
	return 0;
}
void confDMA(void){
	GPDMA_Channel_CFG_Type GPDMACfg;
	/* GPDMA block section -------------------------------------------- */
	/* Disable GPDMA interrupt */
	NVIC_DisableIRQ(DMA_IRQn);
	/* Initialize GPDMA controller */
	GPDMA_Init();
	// Setup GPDMA channel --------------------------------
	// channel 0
	GPDMACfg.ChannelNum = 0;
	// Source memory
	GPDMACfg.SrcMemAddr = (uint32_t)DMASrc_Buffer;
	// Destination memory
	GPDMACfg.DstMemAddr = (uint32_t)DMADest_Buffer;
	// Transfer size
	GPDMACfg.TransferSize = DMA_SIZE;
	// Transfer width
	GPDMACfg.TransferWidth = GPDMA_WIDTH_WORD;
	// Transfer type
	GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_M2M;
	// Source connection - unused
	GPDMACfg.SrcConn = 0;
	// Destination connection - unused
	GPDMACfg.DstConn = 0;
	// Linker List Item - unused
	GPDMACfg.DMALLI = 0;
	// Setup channel with given parameter
	GPDMA_Setup(&GPDMACfg);
}
void Buffer_Verify(void)
{
	uint8_t i;
	uint32_t *src_addr = (uint32_t *)DMASrc_Buffer;
	uint32_t *dest_addr = (uint32_t *)DMADest_Buffer;
	for ( i = 0; i < DMA_SIZE; i++ )
	{
		if ( *src_addr++ != *dest_addr++ )
		{
			while(1){}
		}
	}
	return;
}
void DMA_IRQHandler (void)
{
	// check GPDMA interrupt on channel 0
	if (GPDMA_IntGetStatus(GPDMA_STAT_INT, 0)){ //check interrupt status on channel 0
		// Check counter terminal status
		if(GPDMA_IntGetStatus(GPDMA_STAT_INTTC, 0)){
			// Clear terminate counter Interrupt pending
			GPDMA_ClearIntPending (GPDMA_STATCLR_INTTC, 0);
			Channel0_TC++;
		}
		if (GPDMA_IntGetStatus(GPDMA_STAT_INTERR, 0)){
			// Clear error counter Interrupt pending
			GPDMA_ClearIntPending (GPDMA_STATCLR_INTERR, 0);
			Channel0_Err++;
		}
	}
	return;
}
