#include <ports.h>

/*
*		BOUTONS
*/

void initPortBouton(void){
	int port;
	port = BOUTON_HAUT | BOUTON_BAS | BOUTON_GAUCHE | BOUTON_DROITE | BOUTON_MILIEU;
	P2DIR &= ~(port);
	// Selection du i/o
	P2SEL &= ~(port);
}

int getBoutonHaut(void){
	return (P2IN & BOUTON_HAUT) != BOUTON_HAUT;
}

int getBoutonBas(void){
	return (P2IN & BOUTON_BAS) != BOUTON_BAS;
}

int getBoutonGauche(void){
	return (P2IN & BOUTON_GAUCHE) != BOUTON_GAUCHE;
}

int getBoutonDroite(void){
	return (P2IN & BOUTON_DROITE) != BOUTON_DROITE;
}

int getBoutonMilieu(void){
	return (P2IN & BOUTON_MILIEU) != BOUTON_MILIEU;
}

/*
*		LEDS
*/

void initPortLed(void){
	int port;
	port = LED_HAUT | LED_BAS | LED_GAUCHE | LED_DROITE | LED_MILIEU;
	P1DIR |= port;
	// Selection du i/o
	P1SEL &= ~(port);
	// Sorties mise à 0 (led eteintes)
	P1OUT &= ~(port);
}

void setLedHaut(int etat){
	if(etat == 1){
		P1OUT |= LED_HAUT;
	}
	else{
		P1OUT &= ~(LED_HAUT);
	}
}

void setLedBas(int etat){
	if(etat){
		P1OUT |= LED_BAS;
	}
	else{
		P1OUT &= ~(LED_BAS);
	}
}

void setLedGauche(int etat){
	if(etat){
		P1OUT |= LED_GAUCHE;
	}
	else{
		P1OUT &= ~(LED_GAUCHE);
	}
}

void setLedDroite(int etat){
	if(etat){
		P1OUT |= LED_DROITE;
	}
	else{
		P1OUT &= ~(LED_DROITE);
	}
}

void setLedMilieu(int etat){
	if(etat){
		P1OUT |= LED_MILIEU;
	}
	else{
		P1OUT &= ~(LED_MILIEU);
	}
}

void initSequenceTest2(void){
	int i;

	setLedMilieu(1);
	for(i=0;i<15000;i++);

	//
	setLedHaut(1);
	for(i=0;i<10000;i++);
	//
	setLedGauche(1);
	
	for(i=0;i<10000;i++);
	setLedHaut(0);
	setLedBas(1);
	for(i=0;i<10000;i++);
	setLedGauche(0);
	setLedDroite(1);
	for(i=0;i<10000;i++);
	setLedBas(0);
	setLedHaut(1);
	for(i=0;i<10000;i++);
	setLedDroite(0);
	setLedGauche(1);
	for(i=0;i<10000;i++);

	setLedHaut(0);
	setLedBas(1);
	for(i=0;i<10000;i++);
	setLedGauche(0);
	setLedDroite(1);
	for(i=0;i<10000;i++);
	setLedBas(0);
	//
	for(i=0;i<10000;i++);
	setLedDroite(0);
	//

	for(i=0;i<15000;i++);
	setLedMilieu(0);
}


void initSequenceTest(void){
        int j;
	for(j=0;j<1;j++){
	    int i;
	    P1OUT |= 0x1e;                          // Set P1.0 LED on
	    for (i = 25000; i>0; i--);              // Delay
	    P1OUT &= ~0x1f;                         // Clear P1.0 LED off
	    for (i = 25000; i>0; i--);              // Delay
	    for (i = 25000; i>0; i--);              // Delay
	    P1OUT |= 0x1e;                          // Set P1.0 LED on
	    for (i = 25000; i>0; i--);              // Delay
	    P1OUT &= ~0x1f;                         // Clear P1.0 LED off
	    for (i = 25000; i>0; i--);              // Delay
	    for (i = 25000; i>0; i--);              // Delay
	    P1OUT |= 0x1e;                          // Set P1.0 LED on
	    for (i = 25000; i>0; i--);              // Delay
	    P1OUT &= ~0x1f;                         // Clear P1.0 LED off
	    for (i = 25000; i>0; i--);              // Delay
	    for (i = 25000; i>0; i--);              // Delay
	    P1OUT |= 0x01;                          // Set P1.0 LED on
	    for (i = 25000; i>0; i--);              // Delay
	    P1OUT &= ~0x1f;                         // Clear P1.0 LED off
	    for (i = 25000; i>0; i--);              // Delay
	    for (i = 25000; i>0; i--);              // Delay
	}
	P1OUT = 0;                                // All P1.x reset
}
/*
*		Commandes
*/

void initComPorts(void){
	int port;
	port = ENABLE_GPS | CMD_SWITCH | RESET_SCREEN;
	P4DIR |= port;
	// Selection du i/o
	P4SEL &= ~(port);
	// Sorties mise à 0
	P4OUT &= ~(ENABLE_GPS | CMD_SWITCH);
	// Sorties mise à 1
	P4OUT |= (RESET_SCREEN);
}

void setENABLE_GPS(int etat){
	if(etat == 1){
		P4OUT |= ENABLE_GPS;
	}
	else{
		P4OUT &= ~(ENABLE_GPS);
	}
}

void setCMD_SWITCH(int etat){
	if(etat == 1){
		P4OUT |= CMD_SWITCH;
	}
	else{
		P4OUT &= ~(CMD_SWITCH);
	}
}

void resetScreen(){
	int i=0;
	P4OUT &= ~(RESET_SCREEN);
	for(i=0;i<0x3ff;i++);
	P4OUT |= (RESET_SCREEN);
}


/*
*		DIVERS
*/

void bindBoutonLed(){
	setLedHaut(getBoutonHaut());
	setLedBas(getBoutonBas());
	setLedDroite(getBoutonDroite());
	setLedGauche(getBoutonGauche());
	setLedMilieu(getBoutonMilieu());
}