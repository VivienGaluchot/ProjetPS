#include <ecran.h>
#include <uart.h>

static int etat;

void printe( char *phrase,char ligne,char colonne,char *BGcolor, char *FGcolor)
{
	moveCursor(ligne,colonne);
	setTextBGColour(BGcolor);
	setTextFGColour(FGcolor);
	putstr(phrase);

}

void moveCursor(char ligne, char colonne){

	sendCharTX1(0xff);
	sendCharTX1(0xe4);
	sendCharTX1(0x00);
	sendCharTX1(ligne);
	sendCharTX1(0x00);
	sendCharTX1(colonne);
	waitACK_RX_1();
}

void setTextFGColour(char *couleur){
	sendCharTX1(0xff);
	sendCharTX1(0x7f);
	sendCharTableTX1(couleur,2);
	waitACK_RX_1();
}

void setTextBGColour(char *couleur){
	sendCharTX1(0xff);
	sendCharTX1(0x7e);
	sendCharTableTX1(couleur,2);
	waitACK_RX_1();
}

void putstr(char *phrase){

	sendCharTX1(0x00);
	sendCharTX1(0x06);
	sendStrTX1(phrase);
	sendCharTX1(0x00);
	waitACK_RX_1();
}

void drawRectangle(char x1,char y1,char x2, char y2, char *couleur){

	sendCharTX1(0xff);
	sendCharTX1(0xcf);
	sendCharTX1(0x00);
	sendCharTX1(x1);
	sendCharTX1(0x00);
	sendCharTX1(x2);
	sendCharTX1(0x00);
	sendCharTX1(y1);
	sendCharTX1(0x00);
	sendCharTX1(y2);
	sendCharTableTX1(couleur,2);
	waitACK_RX_1();

}

void drawFilledRectangle(char x1,char y1,char x2, char y2, char *couleur){

	sendCharTX1(0xff);
	sendCharTX1(0xce);
	sendCharTX1(0x00);
	sendCharTX1(x1);
	sendCharTX1(0x00);
	sendCharTX1(x2);
	sendCharTX1(0x00);
	sendCharTX1(y1);
	sendCharTX1(0x00);
	sendCharTX1(y2);
	sendCharTableTX1(couleur,2);
	waitACK_RX_1();

}

void clearScreen(){
	sendCharTX1(0xff);
	sendCharTX1(0xd7);
	waitACK_RX_1();
}

void menu1(){


	drawFilledRectangle(3,124,3,28,fondbleu);  
	drawFilledRectangle(3,124,35,60,fondorange);  
	drawFilledRectangle(3,124,67,92,fondviolet);  
	drawFilledRectangle(3,124,99,124,fondgris);  
	drawRectangle(3,124,3,28,tourbleu);  
	drawRectangle(3,124,35,60,tourorange);  
	drawRectangle(3,124,67,92,tourviolet);  
	drawRectangle(3,124,99,124,tourgris);  
	printe("Enregistrement",2,1,fondbleu,blanc);  
	printe("Navigation",6,1,fondorange,blanc);
	printe("Jesaispasquoi",10,1,fondviolet,blanc);  
	printe("Serial",14,1,fondgris,blanc);  


}


void majmenu1(int i,int iAvant){
	char color[2] ;

	if(iAvant==0) { color[0] = fondbleu[0];	color[1] = fondbleu[1]; }
	if(iAvant==1) { color[0] = fondorange[0];color[1] = fondorange[1]; }
	if(iAvant==2) { color[0] = fondviolet[0];color[1] = fondviolet[1]; }
	if(iAvant==3) { color[0] = fondgris[0];	color[1] = fondgris[1]; }

	drawRectangle(4,123,32*iAvant+4,32*iAvant+27,color); 
	drawRectangle(2,125,32*iAvant+2,32*iAvant+29,noir); 
	drawRectangle(1,126,32*iAvant+1,32*iAvant+30,noir); 
	drawRectangle(0,127,32*iAvant,32*iAvant+31,noir); 

	if(i==0) { color[0] = tourbleu[0];	color[1] = tourbleu[1]; }
	if(i==1) { color[0] = tourorange[0];color[1] = tourorange[1]; }
	if(i==2) { color[0] = tourviolet[0];color[1] = tourviolet[1]; }
	if(i==3) { color[0] = tourgris[0];	color[1] = tourgris[1]; }

	drawRectangle(4,123,32*i+4,32*i+27,color); 
	drawRectangle(2,125,32*i+2,32*i+29,color); 
	drawRectangle(1,126,32*i+1,32*i+30,color); 
	drawRectangle(0,127,32*i,32*i+31,color); 
}


int bouton(int i){
	int j;

	// PUSH
	if(P2IN == 0x1E) { 
		P1OUT |=0x01;
		clearScreen();
		for (j = 25000; j>0; j--);// Delay
	}else{ 
	P1OUT = 0;
	}

	// TOP
	if(P2IN == 0x1D){
		P1OUT |=0x04;
		if(etat==0){i--;}
		for (j = 25000; j>0; j--);// Delay
	}else{ 
		P1OUT = 0;
	}

	//RIGHT
	if(P2IN == 0x0F) { 
		P1OUT |=0x08;
		if (i==0 && etat == 0){
			affichageEnregistrement();
			etat=1;
		}
	}else{ 
		P1OUT = 0;
	}

	//BOTTOM
	if(P2IN == 0x1B) {
		P1OUT |=0x02; 
		if(etat==0){i++;}
		for (j = 25000; j>0; j--);// Delay
	}else{ 
		P1OUT = 0;
	}

	//LEFT     
	if(P2IN == 0x17) { 
		P1OUT |=0x10;
		if( etat == 1) 
		{
			clearScreen() ;
			etat = 0;
			menu1();
			majmenu1(0,8);
		}
	}else{ 
		P1OUT = 0;
	}

	if(etat==1){
		majaffichageEnregistrement("N 48°12\'13\"","E 13°45\'05\"","175 m","5 Km/h","15:11");
	}

	return (i+8)%4;
}


void affichageEnregistrement(){
	clearScreen();

	drawRectangle(0,127,0,127,bleuclair);
	drawRectangle(1,126,1,126,blert);
	drawRectangle(2,125,2,125,vert);
	drawRectangle(83,83,3,124,blert);
	drawRectangle(2,83,62,94,blert);
	drawRectangle(3,82,61,124,blert);
	drawRectangle(3,82,93,3,blert);


	underline();
	printe("Position",1,1,noir,bleuclair);
	underline();
	printe("Altitude",8,1,noir,bleuclair);
	underline();
	printe("Vitesse",12,1,noir,bleuclair);

}

void majaffichageEnregistrement(char* coord1, char* coord2, char* altitude, char* vitesse,char* heure){

	printe(coord1,3,1,noir,vert);
	printe(coord2,5,1,noir,vert);
	printe(altitude,10,2,noir,vert);
	printe(vitesse,14,2,noir,vert);
	printe(heure,1,12,noir,vert);

}

void initScreen(){

	etat=0;

	sendCharTX1(0x00);	//bit 0 pour que ca marche

	//retourne l'ecran
	sendCharTX1(0xff);
	sendCharTX1(0x68);
	sendCharTX1(0x00);
	sendCharTX1(0x01);
	waitACK_RX_1();

	clearScreen();
}

void underline(){

	sendCharTX1(0xff);
	sendCharTX1(0x73);
	sendCharTX1(0x00);
	sendCharTX1(0x01);
	waitACK_RX_1();
}