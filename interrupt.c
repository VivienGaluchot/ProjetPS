#include <interrupt.h>

void initIt(void){
	initTimer_A();
	initItP2();

	setItRx0(0);
	setItTx0(0);
	setItRx1(0);
	setItTx1(0);
	setTimer_A(0);
	setItP2(0);

	_EINT(); // enable interrupt
}

// ---------------------------------- Accesseurs ----------------------------------
void setFuncRx0(void (*func)(void)){
	funcRx0 = func;
}

void setFuncTx0(void (*func)(void)){
	funcTx0 = func;
}

void setFuncRx1(void (*func)(void)){
	funcRx1 = func;
}

void setFuncTx1(void (*func)(void)){
	funcTx1 = func;
}

void setFuncTimer_A(void (*func)(void)){
	funcTimerA = func;
}

void setFuncP2(void (*func)(void)){
	funcP2 = func;
}

// ---------------------------------- Uart ----------------------------------
void setItRx0(int etat){
	if(etat) ME1 |= URXE0;
	else ME1 &= ~URXE0;
}

void setItTx0(int etat){
	if(etat) ME1 |= UTXE0;
	else ME1 &= ~UTXE0;
}

void setItRx1(int etat){
	if(etat) ME2 |= URXE1;
	else ME2 &= ~URXE1;
}

void setItTx1(int etat){
	if(etat) ME2 |= UTXE1;
	else ME2 &= ~UTXE1;
}

// ---------------------------------- Timer ----------------------------------
void initTimer_A(void){
	TACTL = TASSEL0 | TACLR;              // ACLK, clear TAR
	CCTL0 = CCIE;                         // CCR0 interrupt enabled
	CCR0 = 18000;                         // freq = (18k / 32khz)*2 ~ 1.1s
}

void setTimer_A(int etat){
	if(etat) TACTL |= MC0|MC1;            // Start Timer_a in updown mode
	else TACTL &= ~(MC0|MC1);             // Stop Timer_a
}

// ---------------------------------- Ports ----------------------------------
void initItP2(void){
	int ports = BOUTON_HAUT | BOUTON_BAS | BOUTON_GAUCHE | BOUTON_DROITE | BOUTON_MILIEU;
	P2IES |= ports; //high-to-low transition (bit a 1) pour les boutons
	P2IFG = 0; //interrupt flag
}

void setItP2(int etat){
	int ports = BOUTON_HAUT | BOUTON_BAS | BOUTON_GAUCHE | BOUTON_DROITE | BOUTON_MILIEU;
	if(etat) P2IE |= ports;
	else P2IE &= ~ports;
}

// --------------------------- Fonctions interruption--------------------------
void usart0_rx (void) __interrupt[UART0RX_VECTOR]{
	(*funcRx0)();
}

void usart0_tx (void) __interrupt[UART0TX_VECTOR]{
	(*funcTx0)();
}

void usart1_rx (void) __interrupt[UART1RX_VECTOR]{
	(*funcRx1)();
}

void usart1_tx (void) __interrupt[UART1TX_VECTOR]{
	(*funcTx1)();
}

void Timer_A (void) __interrupt[TIMERA0_VECTOR]{
	(*funcTimerA)();
}

void Port_2 (void) __interrupt[PORT2_VECTOR]{
	(*funcP2)();
	P2IFG = 0; //interrupt flag
}
