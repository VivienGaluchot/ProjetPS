#include <moduleGPS.h>

void initGPS(){
	setENABLE_GPS(1);
	initModule0();
	listenGPS(1);
	_EINT();
}

void traiterDataGPS(){
	//
}