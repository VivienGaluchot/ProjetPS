#include <__cross_studio_io.h>
#include  <msp430x16x.h>
#include <ecran.h>

int next;
int ACK;

void envoicommande(char commande[],int taille)
{
  int i;
  i=0;
  next=1;
  while(i<taille)
  {
    if (next){
    next = 0;
    TXBUF1 =  commande[i];
    i++;
    }
  }
}

void envoibyte(int byte){
  int i;
  i=0;
  next=1;
  while(i<1)
  {
    if (next){
    next = 0;
    TXBUF1 = byte;
    i++;
    }
  }
}


void printe( char *phrase,int ligne,int colonne)
{
  char MoveCursor[3] = {0xFF,0xe4,0x00};
  int i, once;
  once=0;
  i=-2;

  envoicommande(MoveCursor,3);
  envoibyte(ligne);
  envoibyte(0x00);
  envoibyte(colonne);
  while(!ACK);
  ACK=0;

  while(phrase[i]!='$')
  {
    if(next && i==-2){
      next = 0;
      TXBUF1 = 0x00;
      i++;
     }
      if(next && i==-1){
      next = 0;
      TXBUF1 = 0x06;
      i++;
     }
    if (next && i>=0)
    {
      next = 0;
      TXBUF1 =  phrase[i];
      i++;
    }
  }
  while(!once){
    if(next){
      next = 0;
      TXBUF1 = 0x00;
      once=1;
    }
   }
  while(!ACK);
   ACK=0;
}