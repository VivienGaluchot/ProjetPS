#include <uart.h>

void initModule0(){
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

void initModule1(){
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

void initTimerGps(){
	WDTCTL = WDTPW + WDTHOLD;             // Stop WDT
	TACTL = TASSEL0 + TACLR;              // ACLK, clear TAR
	CCTL0 = CCIE;                         // CCR0 interrupt enabled
	CCR0 = 20000;
	TACTL |= MC0|MC1;                     // Start Timer_a in updown mode
}

void resetTimer(){
	TACTL &= ~(MC0|MC1); //stop timer
	TAR = 0;
	TACTL |= MC0|MC1; // start timer
}

void connectUsbToScreen(int etat){
	if(etat){
		if(itMode & LISTEN_GPS)
			connectGPS(0);

		itMode |= USB_TO_SCREEN;

		setCMD_SWITCH(1);

		setIT_RX_0(1);
		setIT_RX_1(1);
		setIT_TX_0(1);
		setIT_TX_1(1);
	}
	else{
		setIT_RX_0(0);
		setIT_RX_1(0);
		setIT_TX_0(0);
		setIT_TX_1(0);

		itMode &= ~USB_TO_SCREEN;
	}
}

void connectGPS(int etat){
	if(etat){
		if(itMode & USB_TO_SCREEN)
			connectUsbToScreen(0);

		itMode |= LISTEN_GPS;
		iBuff = 0;
		setCMD_SWITCH(0);
		setIT_RX_0(1);
		setIT_TX_0(1);
	}
	else{
		setIT_RX_0(0);
		setIT_TX_0(0);
		itMode &= ~LISTEN_GPS;
	}
}

void connectScreen(int etat){
	if(etat){
		itMode |= CONNECT_SCREEN;

		setIT_RX_1(1);
		setIT_TX_1(1);
	}
	else{
		setIT_RX_1(0);
		setIT_TX_1(0);

		itMode &= ~CONNECT_SCREEN;
	}
}

/*
*	INTERRUPTIONS Sets
*/

void setIT_RX_0(int etat){
	if(etat){
		IFG1 &= ~URXIFG0;                 // flag reset
		IE1 |= URXIE0;
	}
	else{
		IE1 &= ~URXIE0;
	}
}

void setIT_TX_0(int etat){
	if(etat){
		IFG1 &= ~UTXIFG0;                 // flag reset
		IE1 |= UTXIE0;
	}
	else{
		IE1 &= ~UTXIE0;
	}
}

void setIT_RX_1(int etat){
	if(etat){
		IFG2 &= ~URXIFG1;                 // flag reset
		IE2 |= URXIE1;
	}
	else{
		IE2 &= ~URXIE1;
	}
}

void setIT_TX_1(int etat){
	if(etat){
		IFG2 &= ~UTXIFG1;                 // flag reset
		IE2 |= UTXIE1;
	}
	else{
		IE2 &= ~UTXIE1;
	}
}

void waitACK_RX_1(){
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

void sendCharTX1(char valeur){
	nextT1 = 0;
	TXBUF1 = valeur;
	while(nextT1 == 0);
}
/*
*	Interruptions
*/

// MODULE 0 RX
void usart0_rx (void) __interrupt[UART0RX_VECTOR]
{
	// 1 - usb connecté à l'écran
	if(itMode & USB_TO_SCREEN)
		TXBUF1 = RXBUF0;
	
	// 2 - écoute du gps
	if((itMode & LISTEN_GPS) && (iBuff < BUFF_SIZE)){
		buffer[iBuff] = RXBUF0;
		iBuff++;
	}
}

// MODULE 0 TX
void usart0_tx (void) __interrupt[UART0TX_VECTOR]
{
	if((itMode & USB_TO_SCREEN) || (itMode & LISTEN_GPS))
		nextT0 = 1;
}

// MODULE 1 RX
void usart1_rx (void) __interrupt[UART1RX_VECTOR]
{
	// 1 - usb connecté à l'écran
	if(itMode & USB_TO_SCREEN)
		TXBUF0 = RXBUF1;

	// 3 - ecran connecté
	if(itMode & CONNECT_SCREEN)
		IT_R1_ACK = 1;
}

// MODULE 1 TX
void usart1_tx (void) __interrupt[UART1TX_VECTOR]
{
	// 3 - ecran connecté
	if(itMode & CONNECT_SCREEN)
		nextT1 = 1;
}

// Timer A0 interrupt service routine
void Timer_A (void) __interrupt[TIMERA0_VECTOR]
{
	// 2 - écoute du gps
	if(itMode & LISTEN_GPS && iBuff>0){
		setIT_TX_0(0); // desactive l'interruption TX0

		traiterDataGPS(buffer,iBuff);
		iBuff = 0;

		setIT_TX_0(1); // reactive l'interruption TX0
	}
}