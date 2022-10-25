#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>


#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"

#define OUTPUT 	(uint8_t) 1
#define INPUT 	(uint8_t) 0

#define PIN_22 ((uint32_t)(1<<22))
#define PORT_ZERO (uint8_t)		0
#define PORT_ONE (uint8_t) 		1
#define PORT_TWO (uint8_t) 		2
#define PORT_THREE (uint8_t) 	3

void config_GPIO(void);
void config_timer(void);

void TIMER0_IRQHandler(void){

	if(GPIO_ReadValue(PORT_ZERO) & PIN_22){
		GPIO_ClearValue(PORT_ZERO, PIN_22);
	} else {
		GPIO_SetValue(PORT_ZERO, PIN_22);
	}

	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);//Clereamos el flag de interrupcion
}

int main(void){

	config_GPIO();
	config_timer();
	GPIO_SetValue(PORT_ZERO, PIN_22);

	while(1){}

	return 0;
}


void config_GPIO(){
	PINSEL_CFG_Type pin_configuration;

	pin_configuration.Portnum 	= PINSEL_PORT_0; //Los siguientes valores estan definidos en el driver
	pin_configuration.Pinnum 	= PINSEL_PIN_22;
	pin_configuration.Pinmode 	= PINSEL_PINMODE_PULLUP;
	pin_configuration.Funcnum 	= PINSEL_FUNC_0;
	pin_configuration.OpenDrain = PINSEL_PINMODE_NORMAL;
	//Hasta aca solo creamos y cargamos la variable que va a utilizar la funcion que configura al periferico

	PINSEL_ConfigPin(&pin_configuration); //Esta es la funcion que configura al periferico. Necesita la variable por referencia

	GPIO_SetDir(PORT_ZERO, PIN_22, OUTPUT);	 //Una vez se configura el periferico, se setea la direccion del pin
	//GPIO_SetDir(pin_configuration.Portnum, PIN_22, OUTPUT); Tambien se podria cargar de esta manera

	return;
}

void config_timer(){
	TIM_TIMERCFG_Type struct_config;
	TIM_MATCHCFG_Type struct_match; //Nos permite configurar uno de los match del timer

	struct_config.PrescaleOption 	= 	TIM_PRESCALE_USVAL; //Permite definir si el valor
	//que cargaremos en el PrescaleValue sera el valor que entra en el registro del prescaler (menos 1)
	//o si ese valor sera un valor de tiempo (por ej, 100 microseg, como en nuestro caso)
	//De esta manera, obtenemos una base de tiempo de 100microseg con el desborde del prescaler
	//Hay tiempos que no pueden ser conseguidos segun a la frecuencia que tenga el reloj que usa el timer
	//El driver no nos advierte sobre esos errores. Debemos ser cautelosos
	struct_config.PrescaleValue 	= 	100; //El prescaler tendra un desborde cada 100us
											 //El timer aumenta cada desborde del prescaler

	struct_match.MatchChannel 		= 	0; //Debe estar en el rango 0..3
	struct_match.IntOnMatch 		= 	ENABLE;
	struct_match.ResetOnMatch 		= 	ENABLE;
	struct_match.StopOnMatch		=	DISABLE;
	struct_match.ExtMatchOutputType	=	TIM_EXTMATCH_NOTHING;
	struct_match.MatchValue 		= 	1000;

	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &struct_config);
	TIM_ConfigMatch(LPC_TIM0, &struct_match);

	TIM_Cmd(LPC_TIM0, ENABLE); //Habilitamos el TMR0

	NVIC_EnableIRQ(TIMER0_IRQn);

	return;


}







