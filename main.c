#include <msp430x16x.h>
#include <ports.h>
#include <moduleGPS.h>
#include <ecran.h>


void main(void)
{
	
	initPortLed();
	initPortBouton(); 
	initComPorts();

	resetScreen();
	initModule1(); // A mettre dans initScreen() si possible, avec resetScreen()
	initGPS();
	initSequenceTest();

	// Commande de l'écran par l'usb
	//connectUsbToScreen(1);
	// Demarage du module GPS
	
	connectGPS(1);
	connectScreen(1);

	_EINT();

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
//        printe("C'est le test",0,0);

	while(1){
		bindBoutonLed();
	}
}