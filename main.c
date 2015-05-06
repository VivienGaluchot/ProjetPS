#include <msp430x16x.h>

#include <ports.h>
#include <interrupt.h>
#include <uart.h>
#include <moduleGPS.h>
#include <ecran.h>

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
		// Mettre le cpu en pause la
		traiterDataGPS();
		// Mettre a jour l'affichage ici après le changement des données
		majScreen();
		// CPU en veille
		LPM0;
	}
}
