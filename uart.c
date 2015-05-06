#include <uart.h>

void initModule0(void){
	UCTL0 |= SWRST;                      // Initialize USART state machine

	// Clock du module
	BCSCTL1 &= ~XT2OFF;                   // XT2 activé
	BCSCTL2 |= SELS;                      // SMCLOCL = xt2
	UTCTL0 = SSEL1;                       // UCLK = SMCLK

	// Donnnées
	UCTL0 = CHAR;                         // 8-bit character

	// Baud rate sur 16 bits
	UBR00 = 0x41;                         // Octet0
	UBR10 = 0x03;                         // Octet1
	UMCTL0 = 0x92;                        // modulation

	// Ports
	P3SEL |= (PORT_TX_0 | PORT_RX_0);     //port 3 selection, 1 pour 3.4 & 3.5
	P3DIR |= PORT_TX_0;                   // P3.4 output direction
	P3DIR &= ~(PORT_RX_0);                // P3.5 input direction

	ME1 |= UTXE0 | URXE0;

	UCTL0 &= ~SWRST;                      // End initialize USART state machine
}

void initModule1(void){
	UCTL1 |= SWRST;                      // Initialize USART state machine

	// Clock du module
	BCSCTL1 &= ~XT2OFF;                   // XT2 activé
	BCSCTL2 |= SELS;                      // SMCLOCL = xt2
	UTCTL1 = SSEL1;                       // UCLK = SMCLK

	// Donnnées
	UCTL1 = CHAR;                         // 8-bit character

	// Baud rate sur 16 bits
	UBR01 = 0x41;                         // Octet0
	UBR11 = 0x03;                         // Octet1
	UMCTL1 = 0x92;                        // modulation

	// Ports
	P3SEL |= (PORT_TX_1 | PORT_RX_1);     //port 3 selection, 1 pour 3.6 & 3.7
	P3DIR |= PORT_TX_1;                   // P3.6 output direction
	P3DIR &= ~(PORT_RX_1);                // P3.7 input direction

	ME2 |= UTXE1 | URXE1;

	UCTL1 &= ~SWRST;                      // End initialize USART state machine
}

void connectUsbToScreen(int etat){
	if(etat){
		setCMD_SWITCH(1);

		setFuncRx0(&usbToSreenURX0);
		setFuncRx1(&usbToSreenURX1);
		setItRx0(1);
		setItRx1(1);
	}
	else{
		setItRx0(0);
		setItRx1(0);
		setFuncRx0(&rien);
		setFuncRx1(&rien);
	}
}

void connectGPS(int etat){
	if(etat){
		setCMD_SWITCH(0);

		// la fonctionRX0 est mise dans le module GPS
		setFuncTx0(&envoiUTX0);
		setItRx0(1);
		setItTx0(1);
	}
	else{
		setItRx0(0);
		setItTx0(0);
		setFuncRx0(&rien);
		setFuncTx0(&rien);
	}
}

void connectScreen(int etat){
	if(etat){
		setFuncRx1(&screenURX1);
		setFuncTx1(&envoiUTX1);
		setItRx1(1);
		setItTx1(1);
	}
	else{
		setItRx1(0);
		setItTx1(0);
		setFuncRx1(&rien);
		setFuncTx1(&rien);
	}
}

void waitACK_RX_1(void){
	unsigned long i = 0;
	IT_R1_ACK = 0;
	while(i<0xffff && IT_R1_ACK==0) i++;
}

void sendStrTX0(char* str){
	while(*str){
		nextT0 = 0;
		TXBUF0 = *str++;
		while(!nextT0);
	}
}

void sendStrTX1(char* str){
	while(*str){
		nextT1 = 0;
		TXBUF1 = *str++;
		while(!nextT1);
	}
}

void sendCharTableTX0(char* table, int n){
	int i;
	i = 0;
	while(i<n){
		nextT0 = 0;
		TXBUF0 = table[i];
		while(nextT0 == 0);
		i++;
		
	}
}

void sendCharTableTX1(char* table, int n){
	int i;
	i = 0;
	while(i<n){
		nextT1 = 0;
		TXBUF1 = table[i];
		while(nextT1 == 0);
		i++;
	}
}

void sendCharTX0(char valeur){
	nextT0 = 0;
	TXBUF0 = valeur;
	while(nextT0 == 0);
}

void sendCharTX1(char valeur){
	nextT1 = 0;
	TXBUF1 = valeur;
	while(nextT1 == 0);
}

/*
*	Fonctions d'iterruptions
*/

void usbToSreenURX0(void){
	TXBUF1 = RXBUF0;
}

void envoiUTX0(void){
	nextT0 = 1;
}

void usbToSreenURX1(void){
	TXBUF0 = RXBUF1;
}

void screenURX1(void){
	IT_R1_ACK = 1;
}

void envoiUTX1(void){
	nextT1 = 1;
}
