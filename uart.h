#ifndef MODULE_GPS_H
#define MODULE_GPS_H

#include <msp430x16x.h>
#include <ports.h>
#include <interrupt.h>
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
#define CONNECT_SCREEN 	0x04		// bit 3 - connecté a l'écran
static int itMode = 0;

static int nextT0 = 0;
static int nextT1 = 0;
static int IT_R0 = 0;
static int IT_R1_ACK = 0;

//Buffer de reception GPS
#define BUFF_SIZE 512
static char buffer0[BUFF_SIZE];
static char buffer1[BUFF_SIZE];
static int iBuff0 = 0;
static int iBuff1 = 0;
static char useBuffer = 0;

void initModule0(void);
void initModule1(void);
void initTimerGps(void);
void resetTimer(void);

// Atention, connecter l'usb à l'écran ou ecouter le gps ne peuvent se faire en meme temps
void connectUsbToScreen(int etat);
void connectGPS(int etat);
void connectScreen(int etat);

void waitACK_RX_1(void);

void sendStrTX0(char* str);
void sendStrTX1(char* str);

void sendCharTableTX0(char* table, int n);
void sendCharTableTX1(char* table, int n);
void sendCharTX1(char valeur);

// Fonctions d'iterruptions
void usbToSreenURX0(void);
void gpsURX0(void);
void envoiUTX0(void);
void usbToSreenURX1(void);
void screenURX1(void);
void envoiUTX1(void);
void vidageBuffer(void);

#endif
