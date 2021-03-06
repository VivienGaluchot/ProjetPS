#include <msp430x16x.h>

#include <ports.h>
#include <interrupt.h>
#include <uart.h>
#include <moduleGPS.h>
#include <ecran.h>
#include <data.h>

void main(void)
{
	// Initialisation des i/o
	initPortLed();
	initPortBouton();
	initComPorts();

	// Initialisation du module d'interruptions
	initIt();

	// Initialisation des modules UART
	initModule0();
	initModule1();

	// Initialisation du module GPS
	initGPS();

	// Ecran
	resetScreen();
	initSequenceTest();
	initScreen();

	while(1){
		traiterDataGPS();
		majScreen();
		// CPU en pause jusqu'a la prochaine interruption
		LPM0;
	}
}
