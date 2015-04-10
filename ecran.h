#ifndef _ECRAN_H
#define _ECRAN_H

extern int next;
extern int ACK;

void envoicommande( char *commande,int taille);
void envoibyte( int byte);
void printe( char *phrase,int ligne,int colonne);

#endif