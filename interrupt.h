#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <msp430x16x.h>
#include <ports.h>

/*
*	Le but de ce module est d'avoir la possibilitée de changer
*	en cours de programme la fonction appelée lors d'une iterruptions
*/

void rien(void);
void initIt(void);

// Pointeurs de fonctions
static void (*funcRx0)(void);
static void (*funcTx0)(void);
static void (*funcRx1)(void);
static void (*funcTx1)(void);
static void (*funcTimerA)(void);

static void (*funcPressBoutonHaut)(void);
static void (*funcPressBoutonBas)(void);
static void (*funcPressBoutonGauche)(void);
static void (*funcPressBoutonDroite)(void);
static void (*funcPressBoutonMilieu)(void);
static void (*funcReleaseBoutonHaut)(void);
static void (*funcReleaseBoutonBas)(void);
static void (*funcReleaseBoutonGauche)(void);
static void (*funcReleaseBoutonDroite)(void);
static void (*funcReleaseBoutonMilieu)(void);

void setFuncRx0(void (*func)(void));
void setFuncTx0(void (*func)(void));
void setFuncRx1(void (*func)(void));
void setFuncTx1(void (*func)(void));
void setFuncTimer_A(void (*func)(void));
void setFuncPressBoutonHaut(void (*func)(void));
void setFuncPressBoutonBas(void (*func)(void));
void setFuncPressBoutonGauche(void (*func)(void));
void setFuncPressBoutonDroite(void (*func)(void));
void setFuncPressBoutonMilieu(void (*func)(void));
void setFuncReleaseBoutonHaut(void (*func)(void));
void setFuncReleaseBoutonBas(void (*func)(void));
void setFuncReleaseBoutonGauche(void (*func)(void));
void setFuncReleaseBoutonDroite(void (*func)(void));
void setFuncReleaseBoutonMilieu(void (*func)(void));

// Uart
void setItRx0(int etat);
void setItTx0(int etat);
void setItRx1(int etat);
void setItTx1(int etat);

// Timer
void initTimer_A(void);
void setTimer_A(int etat);

// Ports
void initItP2(void);
void setItP2(int etat);

// Fonctions interruption
void usart0_rx(void);
void usart0_tx(void);
void usart1_rx(void);
void usart1_tx(void);
void Timer_A(void);
void Port_2(void);

#endif
