#ifndef _ECRAN_H
#define _ECRAN_H

#include <__cross_studio_io.h>
#include  <msp430x16x.h>

extern int next;
extern int ACK;

void envoicommande( char *commande,int taille);
void envoibyte( int byte);
void printe( char *phrase,int ligne,int colonne);

#endif