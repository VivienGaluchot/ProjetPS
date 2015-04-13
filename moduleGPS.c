#include <moduleGPS.h>

void initGPS(){
	setENABLE_GPS(1);
	initTimerGps();
}

void traiterDataGPS(char* data){
	debug_printf("\ntraiterDataGPS\n");
	debug_printf(data);
}
