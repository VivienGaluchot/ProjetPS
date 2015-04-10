#include <moduleGPS.h>

void initGPS(){
	setENABLE_GPS(1);
	initModule0();
	listenGPS(1);
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