#include <msp430x16x.h>
#include <interrupt.h>
#include <uart.h>
#include <ports.h>

void main(void)
{
	int i;

	// Initialisation des ports
	initPortLed();
	initPortBouton();
	initComPorts();

	// Initialisation du module d'interruptions
	initIt();

	// Initialisation du timer


	// Initialisation des modules UART
	initModule0();
	initModule1();
	
	// Main
	bindBoutonLed();
	setItP2(1);

	initSequenceTest();

	while(1){
		i++;
	}
}
