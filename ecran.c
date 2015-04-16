#include <ecran.h>
#include <uart.h>

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

void menu1(int i,int iAvant){


	drawFilledRectangle(3,125,3,29,fondbleu);  
	drawFilledRectangle(3,125,35,61,fondorange);  
	drawFilledRectangle(3,125,67,93,fondviolet);  
	drawFilledRectangle(3,125,99,125,fondgris);  
	drawRectangle(3,125,3,29,tourbleu);  
	drawRectangle(3,125,35,61,tourorange);  
	drawRectangle(3,125,67,93,tourviolet);  
	drawRectangle(3,125,99,125,tourgris);  
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

	drawRectangle(4,124,32*iAvant+4,32*iAvant+28,color); 
	drawRectangle(2,126,32*iAvant+2,32*iAvant+30,noir); 
	drawRectangle(1,127,32*iAvant+1,32*iAvant+31,noir); 
	drawRectangle(0,128,32*iAvant,32*iAvant+32,noir); 

	if(i==0) { color[0] = tourbleu[0];	color[1] = tourbleu[1]; }
	if(i==1) { color[0] = tourorange[0];color[1] = tourorange[1]; }
	if(i==2) { color[0] = tourviolet[0];color[1] = tourviolet[1]; }
	if(i==3) { color[0] = tourgris[0];	color[1] = tourgris[1]; }

	drawRectangle(4,124,32*i+4,32*i+28,color); 
	drawRectangle(2,126,32*i+2,32*i+30,color); 
	drawRectangle(1,127,32*i+1,32*i+31,color); 
	drawRectangle(0,128,32*i,32*i+32,color); 
 }


int bouton(int i){
	int j;
	if(P2IN == 0x1E) { P1OUT |=0x01; clearScreen() ;}else{ P1OUT = 0;}        //PUSH
	if(P2IN == 0x1D) { 										  //TOP
		P1OUT |=0x04;
		i--;
		for (j = 25000; j>0; j--);              // Delay
	}else{ 
		P1OUT = 0;
	}
	if(P2IN == 0x0F) { P1OUT |=0x08;}else{ P1OUT = 0;}        //RIGHT
	if(P2IN == 0x1B) {
		P1OUT |=0x02;										  //BOTTOM
		i++;
		for (j = 25000; j>0; j--);              // Delay
	}else{ 
		P1OUT = 0;
	}        
	if(P2IN == 0x17) { P1OUT |=0x10;}else{ P1OUT = 0;}        //LEFT

	return (i+8)%4;
}
