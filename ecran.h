#ifndef _ECRAN_H
#define _ECRAN_H

#include <__cross_studio_io.h>
#include <msp430x16x.h>
#include <uart.h>
#include <ports.h>

static char clear[] = {0xFF,0xD7};
static char landscapeR[] = {0xFF,0x68, 0x00,0x01};
static char fondBleu[] = {0xFF,0xCE, 0x00,0x00, 0x00,0x00, 0x00,0x80, 0x00,0x80, 0x00,0x4D};
static char rondBleuClair[] = {0xFF,0xCC, 0x00,0x40, 0x00,0x40, 0x00,0x32, 0x04,0xB9};
static char txtHeight[] = {0xFF,0x7B, 0x00,0x02};
static char txtWidth[] = {0xFF,0x7C, 0x00,0x02};
static char putStr[] = {0x00,0x06,'G','P','S',0x00};

static char blanc[]={0xff,0xff};
static char noir[]={0x00,0x00};
static char blert[]={0x0d,0x4b};
static char vert[]={0x05,0xe4};
static char bleuclair[]={0x1c,0xbd};

static char fondviolet[]={0xd0,0xcf};
static char tourviolet[]={0xeb,0xd6};
static char fondbleu[]={0x19,0x71};
static char tourbleu[]={0x1e,0xde};
static char fondorange[]={0xf3,0xa0};
static char tourorange[]={0xf7,0xa0};
static char fondgris[]={0x6b,0x6d};
static char tourgris[]={0xc6,0x18};

void initScreen(void);

static int etat;
static int menu1Item;
static int prevMenu1Item;

// Choses a faire lors de la majScreen
static int ClearScreen_TODO;
static int MajMenu1_TODO;
static int Menu1_TODO;
static int AffichageEnregistrement_TODO;

void majScreen();

void printe( char *phrase,char ligne,char colonne,char *BGcolor, char *FGcolor);
void moveCursor(char ligne,char colonne);
void setTextFGColour(char *couleur);
void setTextBGColour(char *couleur);
void putstr(char *phrase);
void drawRectangle(char x1,char y1,char x2, char y2, char *couleur);
void drawFilledRectangle(char x1,char y1,char x2, char y2, char *couleur);
void clearScreen(void);
void underline(void);

void menu1(void);
void majmenu1(void);
void affichageEnregistrement(void);
void majaffichageEnregistrement(char* coord1, char* coord2, char* altitude, char* vitesse,char* heure);

// Fonctions interruptions
void boutonMilieu(void);
void boutonHaut(void);
void boutonDroit(void);
void boutonBas(void);
void boutonGauche(void);

#endif