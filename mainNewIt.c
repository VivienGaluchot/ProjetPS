#include <msp430x16x.h>
#include <interrupt.h>
#include <ports.h>

void main(void)
{
	int i;

	initPortLed();
	initPortBouton();
	initComPorts();
	initIt();
	
	bindBoutonLed();
	setItP2(1);

	while(1){
		i++;
	}
}
