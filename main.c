#include <msp430x16x.h>
#include <uart.h>
#include <ports.h>
#include <moduleGPS.h>
#include <ecran.h>

void main(void)
{
	char fondBleu[] = {0xFF,0xCE, 0x00,0x00, 0x00,0x00, 0x00,0x80, 0x00,0x80, 0x00,0x4D};
	char rondBleuClair[] = {0xFF,0xCC, 0x00,0x40, 0x00,0x40, 0x00,0x32, 0x04,0xB9};
	char txtHeight[] = {0xFF,0x7B, 0x00,0x03};
	char txtWidth[] = {0xFF,0x7C, 0x00,0x03};
	char putStr[] = {0x00,0x06,'G','P','S',0x00};
	char clr[] = {0xFF,0xD7};

	_EINT(); // enable interrupt

	initPortLed();
	initPortBouton(); 
	initComPorts();

	resetScreen(); // A mettre dans initScreen() si possible

	initModule0();
	initModule1();

	initSequenceTest2();

	initGPS();
	
	connectScreen(1);

	sendCharTX1(0);
	sendCharTableTX1(fondBleu,12);
	waitACK_RX_1();
	sendCharTableTX1(rondBleuClair,10);
	waitACK_RX_1();
	sendCharTableTX1(txtHeight,4);
	waitACK_RX_1();
	sendCharTableTX1(txtWidth,4);
	waitACK_RX_1();
	sendCharTableTX1(putStr,6);
	waitACK_RX_1();
	sendCharTableTX1(clr,2);

	while(1){
		bindBoutonLed();
	}
}
