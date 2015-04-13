#ifndef MODULE_GPS_H
#define MODULE_GPS_H

#include <msp430x16x.h>
#include <ports.h>
#include <moduleGPS.h>

/*
	Ports : TX P3.4 - RX P3.5
	BCSCTL1	 			Basic Clock System Control 1
	XTS 				LFXTCLK 0:Low Freq. / 1: High Freq.
	IFG1				Interrupt Flag 1 (= UART0 Interrupt Flag Register)
	OFIFG				0x02 = 0000 0010
	ME1					UART0 Module Enable Register

	clock XT2 HF		8,000 MHz
	modulation trouvée avec :
	http://www.daycounter.com/Calculators/MSP430-Uart-Calculator.phtml
*/

#define PORT_TX_0	0x10	// P3.4
#define PORT_RX_0	0x20	// P3.5
#define PORT_TX_1	0x40	// P3.6
#define PORT_RX_1	0x80	// P3.7

// mode d'interruptions
#define NO_IT 			0x00		// 0 - stop
#define USB_TO_SCREEN 	0x01		// bit 1 - usb connecté à l'écran
#define LISTEN_GPS 		0x02		// bit 2 - écoute du gps
#define CONNECT_SCEEN 	0x04		// bit 3 - connecté a l'écran
static int itMode = 0;

static int nextT0 = 0;
static int nextT1 = 0;
static int IT_R0 = 0;
static int IT_R1_ACK = 0;

//Buffer de reception GPS
static char bufferUART0[400];
static char bufferUART1[400];
static int iBuff0 = 0;
static int iBuff1 = 0;
static int selBuff = 0;

void initModule0();
void initModule1();
void initTimerGps();

// Atention, connecter l'usb à l'écran ou ecouter le gps ne peuvent se faire en meme temps
void connectUsbToScreen(int etat);
void connectGPS(int etat);
void connectScreen(int etat);

void setIT_RX_0(int etat);
void setIT_TX_0(int etat);
void setIT_RX_1(int etat);
void setIT_TX_1(int etat);

void waitACK_RX_1();

void sendStrTX0(char* str);
void sendStrTX1(char* str);

void sendCharTableTX0(char* table, int n);
void sendCharTableTX1(char* table, int n);
void sendCharTX1(char valeur);

//Iterruptions
void usart0_rx();
void usart0_tx();
void usart1_rx();
void usart1_tx();
void Timer_A();

#endif
