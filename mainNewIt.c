#include <msp430x16x.h>
#include <interrupt.h>
#include <ports.h>

void test(void){
	debug_printf("test");
}

void main(void)
{
	int i;

	initPortLed();
	initPortBouton();
	initComPorts();

	initIt();

	setFuncP2(test);
	setItP2(1);

	while(1){
		i++;
	}
}
