#include <moduleGPS.h>

void initUartGPS(){
	setENABLE_GPS(1);
	setCMD_SWITCH(0);

	initModule0();

	//init du buffer
	iBuff0 = 0;
	iBuff1 = 0;
	selBuff = 0;
}

void activerUartGPS(){
	setEnableModule0(1);
	setIT_RX_0(1);
	// Enable interrupts
	_EINT();
}

void traiterDataGPS(){
	if(iBuff1>200){
		selBuff = 0;
		bufferUART1[iBuff1] = 0;
		//debug_printf("--buff1--");
		debug_printf(bufferUART1);
		iBuff1 = 0;
	}
	else if(iBuff0>200){
		selBuff = 1;
		bufferUART0[iBuff0] = 0;
		//debug_printf("--buff0--");
		debug_printf(bufferUART0);
		iBuff0 = 0;
	}
}

/*
*	INTERRUPTIONS
*/

/*
void usart0_rx (void) __interrupt[UART0RX_VECTOR] 
{
	if(selBuff==1){
		bufferUART1[iBuff1] = RXBUF0;
		iBuff1++;
	}
	else
	{
		bufferUART0[iBuff0] = RXBUF0;
		iBuff0++;
	}
}
*/