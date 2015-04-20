#ifndef PORTS_H
#define PORTS_H

#include <msp430x16x.h>

// BOUTONS

#define BOUTON_HAUT 0x02
#define BOUTON_BAS 0x04
#define BOUTON_GAUCHE 0x08
#define BOUTON_DROITE 0x10
#define BOUTON_MILIEU 0x01

void initPortBouton(void);

int getBoutonHaut(void);
int getBoutonBas(void);
int getBoutonGauche(void);
int getBoutonDroite(void);
int getBoutonMilieu(void);

// LEDS

#define LED_HAUT 0x04
#define LED_BAS 0x02
#define LED_GAUCHE 0x10
#define LED_DROITE 0x08
#define LED_MILIEU 0x01

void initPortLed(void);

void setLedHaut(int etat);
void setLedBas(int etat);
void setLedGauche(int etat);
void setLedDroite(int etat);
void setLedMilieu(int etat);

void allumerLedHaut(void);
void allumerLedBas(void);
void allumerLedGauche(void);
void allumerLedDroite(void);
void allumerLedMilieu(void);
void eteindreLedHaut(void);
void eteindreLedBas(void);
void eteindreLedGauche(void);
void eteindreLedDroite(void);
void eteindreLedMilieu(void);

void initSequenceTest(void);


// Commandes

#define ENABLE_GPS 0x01 // Port 4.0
#define CMD_SWITCH 0x04 // Port 4.2
#define RESET_SCREEN 0x02 // Port 4.1

void initComPorts(void);

void setENABLE_GPS(int etat);
void setCMD_SWITCH(int etat);
void resetScreen(void);


// DIVERS

void bindBoutonLed(void);

#endif
