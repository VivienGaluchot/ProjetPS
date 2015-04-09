#include <msp430x16x.h>
#include <ports.h>
#include <moduleGPS.h>

void main(void)
{
	char clear[] = {0xFF,0xD7};
	char landscapeR[] = {0xFF,0x68, 0x00,0x01};
	char fondBleu[] = {0xFF,0xCE, 0x00,0x00, 0x00,0x00, 0x00,0x80, 0x00,0x80, 0x00,0x4D};
	char rondBleuClair[] = {0xFF,0xCC, 0x00,0x40, 0x00,0x40, 0x00,0x32, 0x04,0xB9};
	char txtHeight[] = {0xFF,0x7B, 0x00,0x03};
	char txtWidth[] = {0xFF,0x7C, 0x00,0x03};
	char putStr[] = {0x00,0x06,'G','P','S',0x00};

	initPortLed();
	initPortBouton(); 
	initComPorts();

	resetScreen();

	initSequenceTest();

	initModule0();
	initModule1();

	_EINT();

	// Commande de l'écran par l'usb
	connectUsbToScreen(1);

	// cette comande marche pas mais si je l'enlève plus rien de marche
	sendCharTableTX1(clear,2);

	sendCharTableTX1(landscapeR,4);
	waitACK_RX_1();
	sendCharTableTX1(fondBleu,12);
	waitACK_RX_1();
	sendCharTableTX1(rondBleuClair,10);
	waitACK_RX_1();
	sendCharTableTX1(txtHeight,4);
	waitACK_RX_1();
	sendCharTableTX1(txtWidth,4);
	waitACK_RX_1();
	sendCharTableTX1(putStr,6);

	while(1){
		bindBoutonLed();
	}
}