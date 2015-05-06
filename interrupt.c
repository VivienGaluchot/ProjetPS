#include <interrupt.h>

void rien(void){}

void initIt(void){
	initTimer_A();
	initItP2();

	setItRx0(0);
	setItTx0(0);
	setItRx1(0);
	setItTx1(0);
	setTimer_A(0);
	setItP2(0);

	setFuncRx0(&rien);
	setFuncTx0(&rien);
	setFuncRx1(&rien);
	setFuncTx1(&rien);
	setFuncTimer_A(&rien);
	setFuncPressBoutonHaut(&rien);
	setFuncPressBoutonBas(&rien);
	setFuncPressBoutonGauche(&rien);
	setFuncPressBoutonDroite(&rien);
	setFuncPressBoutonMilieu(&rien);
	setFuncReleaseBoutonHaut(&rien);
	setFuncReleaseBoutonBas(&rien);
	setFuncReleaseBoutonGauche(&rien);
	setFuncReleaseBoutonDroite(&rien);
	setFuncReleaseBoutonMilieu(&rien);

	_EINT(); // enable interrupt
}

// ---------------------------------- Accesseurs ----------------------------------
void setFuncRx0(void (*func)(void)){ funcRx0 = func; }
void setFuncTx0(void (*func)(void)){ funcTx0 = func; }
void setFuncRx1(void (*func)(void)){ funcRx1 = func; }
void setFuncTx1(void (*func)(void)){ funcTx1 = func; }

void setFuncTimer_A(void (*func)(void)){ funcTimerA = func;}

void setFuncPressBoutonHaut(void (*func)(void)){ funcPressBoutonHaut = func; }
void setFuncPressBoutonBas(void (*func)(void)){ funcPressBoutonBas = func; }
void setFuncPressBoutonGauche(void (*func)(void)){ funcPressBoutonGauche = func; }
void setFuncPressBoutonDroite(void (*func)(void)){ funcPressBoutonDroite = func; }
void setFuncPressBoutonMilieu(void (*func)(void)){ funcPressBoutonMilieu = func; }
void setFuncReleaseBoutonHaut(void (*func)(void)){ funcReleaseBoutonHaut = func; }
void setFuncReleaseBoutonBas(void (*func)(void)){ funcReleaseBoutonBas = func; }
void setFuncReleaseBoutonGauche(void (*func)(void)){ funcReleaseBoutonGauche = func; }
void setFuncReleaseBoutonDroite(void (*func)(void)){ funcReleaseBoutonDroite = func; }
void setFuncReleaseBoutonMilieu(void (*func)(void)){ funcReleaseBoutonMilieu = func; }

// ---------------------------------- Uart ----------------------------------
void setItRx0(int etat){
	if(etat){
		IFG1 &= ~URXIFG0;                 // flag reset
		IE1 |= URXIE0;
	}
	else{
		IE1 &= ~URXIE0;
	}
}

void setItTx0(int etat){
	if(etat){
		IFG1 &= ~UTXIFG0;                 // flag reset
		IE1 |= UTXIE0;
	}
	else{
		IE1 &= ~UTXIE0;
	}
}

void setItRx1(int etat){
	if(etat){
		IFG2 &= ~URXIFG1;                 // flag reset
		IE2 |= URXIE1;
	}
	else{
		IE2 &= ~URXIE1;
	}
}

void setItTx1(int etat){
	if(etat){
		IFG2 &= ~UTXIFG1;                 // flag reset
		IE2 |= UTXIE1;
	}
	else{
		IE2 &= ~UTXIE1;
	}
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
void usart0_rx (void) __interrupt[UART0RX_VECTOR]{ (*funcRx0)();}
void usart0_tx (void) __interrupt[UART0TX_VECTOR]{ (*funcTx0)();}
void usart1_rx (void) __interrupt[UART1RX_VECTOR]{ (*funcRx1)();}
void usart1_tx (void) __interrupt[UART1TX_VECTOR]{ (*funcTx1)();}

void Timer_A (void) __interrupt[TIMERA0_VECTOR]{ (*funcTimerA)();}

void Port_2 (void) __interrupt[PORT2_VECTOR]{
	if(P2IFG & BOUTON_HAUT){
		if((P2IES & BOUTON_HAUT))
			(*funcPressBoutonHaut)();
		else
			(*funcReleaseBoutonHaut)();
	}
	
	if(P2IFG & BOUTON_BAS){
		if((P2IES & BOUTON_BAS))
			(*funcPressBoutonBas)();
		else
			(*funcReleaseBoutonBas)();
	}
	
	if(P2IFG & BOUTON_GAUCHE){
		if((P2IES & BOUTON_GAUCHE))
			(*funcPressBoutonGauche)();
		else
			(*funcReleaseBoutonGauche)();
	}
	
	if(P2IFG & BOUTON_DROITE){
		if((P2IES & BOUTON_DROITE))
			(*funcPressBoutonDroite)();
		else
			(*funcReleaseBoutonDroite)();
	}
	
	if(P2IFG & BOUTON_MILIEU){
		if((P2IES & BOUTON_MILIEU))
			(*funcPressBoutonMilieu)();
		else
			(*funcReleaseBoutonMilieu)();
	}

	P2IES ^= P2IFG; // inversion des transitions
	P2IFG = 0; //interrupt flag
}
