#include <ecran.h>
#include <uart.h>

void printe( char *phrase,int ligne,int colonne)
{


	sendCharTableTX1(MoveCursor,3);
	sendCharTX1((char)ligne);
	sendCharTX1(0x00);
	sendCharTX1((char)colonne);
	waitACK_RX_1();


	sendCharTX1(0x06);
	sendStrTX1(phrase);
	sendCharTX1(0x00);
	waitACK_RX_1();
}