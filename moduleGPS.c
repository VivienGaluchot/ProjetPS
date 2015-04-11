#include <moduleGPS.h>

void initGPS(){
	setENABLE_GPS(1);
	initModule0();
	initTimerGps();
}

void traiterDataGPS(char* data){
	debug_printf("boou\n");
}