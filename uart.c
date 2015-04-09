#include <uart.h>

/*
1) Set SWRST (BIS.B #SWRST,&UxCTL)
2) Initialize all USART registers with SWRST=1 (including UxCTL)
3) Enable USART module via the MEx SFRs (USPIEx)
4) Clear SWRST via software (BIC.B #SWRST,&UxCTL)
5) Enable interrupts (optional) via the IEx SFRs (URXIEx and/or UTXIEx)
*/
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
	nextT0 = 1;
	while(*str){
		if(nextT0){
			nextT0 = 0;
			TXBUF0 = *str++;
		}
	}
}

void sendStrTX1(char* str){
	nextT1 = 1;
	while(*str){
		if(nextT1){
			nextT1 = 0;
			TXBUF1 = *str++;
		}
	}
}

void sendCharTableTX0(char* table, int n){
	int i;
	nextT0 = 1;
	i = 0;
	while(i<n){
		if(nextT0){
			nextT0 = 0;
			TXBUF0 = table[i];
			i++;
		}
	}
}

void sendCharTableTX1(char* table, int n){
	int i,t;
	nextT1 = 1;
	i = 0;
	while(i<n){
		if(nextT1){
			nextT1 = 0;
			//debug_printf("Envoi\n");
			TXBUF1 = table[i];
			i++;
		}
	}
}

/*
*	INTERUPTIONS
*/

void usart0_rx (void) __interrupt[UART0RX_VECTOR] 
{
	TXBUF1 = RXBUF0;
}

void usart0_tx (void) __interrupt[UART0TX_VECTOR] 
{
	nextT0 = 1;
}

void usart1_rx (void) __interrupt[UART1RX_VECTOR] 
{
	//debug_printf("IT RX 1\n");
	TXBUF0 = RXBUF1;

	IT_R1_ACK = 1;
}

void usart1_tx (void) __interrupt[UART1TX_VECTOR] 
{
	nextT1 = 1;
}