#include <ecran.h>

void initScreen(void){
	etat = 1;
	menu1Item = 0;
	prevMenu1Item = 0;

	ClearScreen_TODO = 0;
	MajMenu1_TODO = 0;
	Menu1_TODO = 0;
	AffichageEnregistrement_TODO = 0;
	AffichageBoussole_TODO = 0;
	PassageSerial_TODO = 0;
	RetourSerial_TODO = 0;

	// Interruptions
	bindBoutonLed();
	setFuncReleaseBoutonHaut(&boutonHaut);
	setFuncReleaseBoutonBas(&boutonBas);
	setFuncReleaseBoutonGauche(&boutonGauche);
	setFuncReleaseBoutonDroite(&boutonDroit);
	setFuncReleaseBoutonMilieu(&boutonMilieu);
	setItP2(1);

	connectScreen(1);

	sendCharTX1(0x00);	//bit 0 pour que ca marche

	//retourne l'ecran
	sendCharTX1(0xff);
	sendCharTX1(0x68);
	sendCharTX1(0x00);
	sendCharTX1(0x01);
	waitACK_RX_1();

	clearScreen();

	menu1();
	majmenu1();
}

void majScreen(void){
	if(ClearScreen_TODO){
		clearScreen();
		ClearScreen_TODO = 0;
	}
	if(Menu1_TODO){
		menu1();
		Menu1_TODO = 0;
	}
	if(MajMenu1_TODO){
		majmenu1();
		MajMenu1_TODO = 0;
	}
	if(AffichageEnregistrement_TODO){
		affichageEnregistrement();
		AffichageEnregistrement_TODO = 0;
	}
	if(AffichageBoussole_TODO){
		fondBoussole();
		boussole(63,63,83,14,blanc);
		AffichageBoussole_TODO = 0;
	}
	if(PassageSerial_TODO){
		connectGPS(0);
		connectUsbToScreen(1);
		clearScreen();
		PassageSerial_TODO = 0;
	}
	if(RetourSerial_TODO){
		connectUsbToScreen(0);
		connectGPS(1);
		RetourSerial_TODO = 0;
	}
}

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

void drawLine(char x1,char y1, char x2,char y2, char *couleur){
	sendCharTX1(0xff);
	sendCharTX1(0xd2);
	sendCharTX1(0x00);
	sendCharTX1(x1);
	sendCharTX1(0x00);
	sendCharTX1(y1);
	sendCharTX1(0x00);
	sendCharTX1(x2);
	sendCharTX1(0x00);
	sendCharTX1(y2);
	sendCharTableTX1(couleur,2);
	waitACK_RX_1();
}

void drawCircle(char x, char y, char radius, char*couleur){
	sendCharTX1(0xff);
	sendCharTX1(0xcd);
	sendCharTX1(0x00);
	sendCharTX1(x);
	sendCharTX1(0x00);
	sendCharTX1(y);
	sendCharTX1(0x00);
	sendCharTX1(radius);
	sendCharTableTX1(couleur,2);
	waitACK_RX_1();
}

void drawFilledCircle(char x, char y, char radius, char*couleur){
	sendCharTX1(0xff);
	sendCharTX1(0xcc);
	sendCharTX1(0x00);
	sendCharTX1(x);
	sendCharTX1(0x00);
	sendCharTX1(y);
	sendCharTX1(0x00);
	sendCharTX1(radius);
	sendCharTableTX1(couleur,2);
	waitACK_RX_1();
}

void drawTriangle(char x1,char y1, char x2,char y2, char x3,char y3, char *couleur){
	sendCharTX1(0xff);
	sendCharTX1(0xc9);
	sendCharTX1(0x00);
	sendCharTX1(x1);
	sendCharTX1(0x00);
	sendCharTX1(y1);
	sendCharTX1(0x00);
	sendCharTX1(x2);
	sendCharTX1(0x00);
	sendCharTX1(y2);
	sendCharTX1(0x00);
	sendCharTX1(x3);
	sendCharTX1(0x00);
	sendCharTX1(y3);
	sendCharTableTX1(couleur,2);
	waitACK_RX_1();
}

void fondBoussole(){

	clearScreen();


	drawCircle(63,63,63,fondviolet);
	drawCircle(63,64,63,fondviolet);
	drawCircle(64,64,63,fondviolet);
	drawCircle(64,63,63,fondviolet);

	drawTriangle(0,63,63,72,63,54,tourviolet);
	drawTriangle(127,63,63,72,63,54,tourviolet);
	drawTriangle(63,0,72,63,54,63,tourviolet);
	drawTriangle(63,127,72,63,54,63,tourviolet);

	drawTriangle(63-32,63-32,63+5,63-5,63-5,63+5,tourviolet);
	drawTriangle(63+32,63+32,63+5,63-5,63-5,63+5,tourviolet);

	drawTriangle(63+32,63-32,63+5,63+5,63-5,63-5,tourviolet);
	drawTriangle(63-32,63+32,63+5,63+5,63-5,63-5,tourviolet);
	


	tailleText(2);
	printe("N",0,4,noir,tourviolet);
	// printe("E",4,8,noir,tourviolet);
	printe("S",7,4,noir,tourviolet);
	// printe("W",4,0,noir,tourviolet);
	tailleText(1);
}

void boussole(char x1,char y1,char x2, char y2,char *couleur){

	drawFilledCircle(63,63,9,blanc);


	drawLine( x1-2, y1-9,  x2, y2, couleur);
	drawLine( x1-1, y1-9,  x2, y2, couleur);
	drawLine( x1,   y1-9,  x2, y2, couleur);
	drawLine( x1+1, y1-9,  x2, y2, couleur);
	drawLine( x1+2, y1-9,  x2, y2, couleur);

	drawLine( x1+9, y1+2,  x2, y2, couleur);
	drawLine( x1+9, y1+1,  x2, y2, couleur);
	drawLine( x1+9, y1,    x2, y2, couleur);
	drawLine( x1+9, y1-1,  x2, y2, couleur);
	drawLine( x1+9, y1-2,  x2, y2, couleur);

	drawLine( x1-2, y1+9,  x2, y2, couleur);
	drawLine( x1-1, y1+9,  x2, y2, couleur);
	drawLine( x1,   y1+9,  x2, y2, couleur);
	drawLine( x1+1, y1+9,  x2, y2, couleur);
	drawLine( x1+2, y1+9,  x2, y2, couleur);

	drawLine( x1-9, y1+2,  x2, y2, couleur);
	drawLine( x1-9, y1+1,  x2, y2, couleur);
	drawLine( x1-9, y1,    x2, y2, couleur);
	drawLine( x1-9, y1-1,  x2, y2, couleur);
	drawLine( x1-9, y1-2,  x2, y2, couleur);

	drawLine( x1-3, y1-8,  x2, y2, couleur);
	drawLine( x1-4, y1-8,  x2, y2, couleur);
	drawLine( x1-5, y1-7,  x2, y2, couleur);
	drawLine( x1-6, y1-7,  x2, y2, couleur);
	drawLine( x1-7, y1-6,  x2, y2, couleur);
	drawLine( x1-7, y1-5,  x2, y2, couleur);
	drawLine( x1-8, y1-4,  x2, y2, couleur);
	drawLine( x1-8, y1-3,  x2, y2, couleur);


	drawLine( x1+3, y1-8,  x2, y2, couleur);
	drawLine( x1+4, y1-8,  x2, y2, couleur);
	drawLine( x1+5, y1-7,  x2, y2, couleur);
	drawLine( x1+6, y1-7,  x2, y2, couleur);
	drawLine( x1+7, y1-6,  x2, y2, couleur);
	drawLine( x1+7, y1-5,  x2, y2, couleur);
	drawLine( x1+8, y1-4,  x2, y2, couleur);
	drawLine( x1+8, y1-3,  x2, y2, couleur);

	drawLine( x1-3, y1+8,  x2, y2, couleur);
	drawLine( x1-4, y1+8,  x2, y2, couleur);
	drawLine( x1-5, y1+7,  x2, y2, couleur);
	drawLine( x1-6, y1+7,  x2, y2, couleur);
	drawLine( x1-7, y1+6,  x2, y2, couleur);
	drawLine( x1-7, y1+5,  x2, y2, couleur);
	drawLine( x1-8, y1+4,  x2, y2, couleur);
	drawLine( x1-8, y1+3,  x2, y2, couleur);


	drawLine( x1+3, y1+8,  x2, y2, couleur);
	drawLine( x1+4, y1+8,  x2, y2, couleur);
	drawLine( x1+5, y1+7,  x2, y2, couleur);
	drawLine( x1+6, y1+7,  x2, y2, couleur);
	drawLine( x1+7, y1+6,  x2, y2, couleur);
	drawLine( x1+7, y1+5,  x2, y2, couleur);
	drawLine( x1+8, y1+4,  x2, y2, couleur);
	drawLine( x1+8, y1+3,  x2, y2, couleur);
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

void clearScreen(void){
	sendCharTX1(0xff);
	sendCharTX1(0xd7);
	waitACK_RX_1();
}

void menu1(void){


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
	printe("Boussole",10,1,fondviolet,blanc);  
	printe("Serial",14,1,fondgris,blanc); 




}


void majmenu1(void){
	char color[2] ;

	if(prevMenu1Item==0) { color[0] = fondbleu[0];	color[1] = fondbleu[1]; }
	else if(prevMenu1Item==1) { color[0] = fondorange[0];color[1] = fondorange[1]; }
	else if(prevMenu1Item==2) { color[0] = fondviolet[0];color[1] = fondviolet[1]; }
	else if(prevMenu1Item==3) { color[0] = fondgris[0];	color[1] = fondgris[1]; }

	drawRectangle(4,123,32*prevMenu1Item+4,32*prevMenu1Item+27,color); 
	drawRectangle(2,125,32*prevMenu1Item+2,32*prevMenu1Item+29,noir); 
	drawRectangle(1,126,32*prevMenu1Item+1,32*prevMenu1Item+30,noir); 
	drawRectangle(0,127,32*prevMenu1Item,32*prevMenu1Item+31,noir); 

	if(menu1Item==0) { color[0] = tourbleu[0];	color[1] = tourbleu[1]; }
	else if(menu1Item==1) { color[0] = tourorange[0];color[1] = tourorange[1]; }
	else if(menu1Item==2) { color[0] = tourviolet[0];color[1] = tourviolet[1]; }
	else if(menu1Item==3) { color[0] = tourgris[0];	color[1] = tourgris[1]; }

	drawRectangle(4,123,32*menu1Item+4,32*menu1Item+27,color); 
	drawRectangle(2,125,32*menu1Item+2,32*menu1Item+29,color); 
	drawRectangle(1,126,32*menu1Item+1,32*menu1Item+30,color); 
	drawRectangle(0,127,32*menu1Item,32*menu1Item+31,color); 

	prevMenu1Item = menu1Item;
}

void affichageEnregistrement(void){
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

	majaffichageEnregistrement("N 48°12\'13\"","E 13°45\'05\"","175 m","5 Km/h","15:11");
}

void majaffichageEnregistrement(char* coord1, char* coord2, char* altitude, char* vitesse,char* heure){
	printe(coord1,3,1,noir,vert);
	printe(coord2,5,1,noir,vert);
	printe(altitude,10,2,noir,vert);
	printe(vitesse,14,2,noir,vert);
	printe(heure,1,12,noir,vert);
}

void underline(void){

	sendCharTX1(0xff);
	sendCharTX1(0x73);
	sendCharTX1(0x00);
	sendCharTX1(0x01);
	waitACK_RX_1();
}

void tailleText(char x){
	sendCharTX1(0xff);
	sendCharTX1(0x7c);
	sendCharTX1(0x00);
	sendCharTX1(x);
	waitACK_RX_1();

	sendCharTX1(0xff);
	sendCharTX1(0x7b);
	sendCharTX1(0x00);
	sendCharTX1(x);
	waitACK_RX_1();
}

// Fonctions interruptions
void boutonMilieu(void){
	ClearScreen_TODO = 1;
	eteindreLedMilieu();
}

void boutonHaut(void){
	if(etat==1 || etat==2 || etat==3 || etat==4){
		menu1Item--;
		menu1Item = (menu1Item+8)%4;
		etat= menu1Item+1;
		MajMenu1_TODO = 1;
	}
	eteindreLedHaut();
}

void boutonDroit(void){
	if (menu1Item==0 && etat == 1){
		AffichageEnregistrement_TODO = 1;
		etat=11;
	}
	if (menu1Item==2 && etat == 3){
		AffichageBoussole_TODO = 1;
		etat=31;
	}
	if (menu1Item==3 && etat == 4){
		PassageSerial_TODO = 1;
		etat=41;
	}
	eteindreLedDroite();
}

void boutonBas(void){
	if(etat==1 || etat==2 || etat==3 || etat==4 ){
		menu1Item++;
		menu1Item = menu1Item%4;
		etat= menu1Item+1;
		MajMenu1_TODO = 1;
	}
	eteindreLedBas();
}

void boutonGauche(void){
	if(etat == 11){
		ClearScreen_TODO = 1;
		etat = 1;
		Menu1_TODO = 1;
		MajMenu1_TODO = 1;
	}
	if(etat == 31){
		ClearScreen_TODO = 1;
		etat = 3;
		Menu1_TODO = 1;
		MajMenu1_TODO = 1;
	}
	if(etat == 41){
		ClearScreen_TODO = 1;
		etat = 4;
		Menu1_TODO = 1;
		MajMenu1_TODO = 1;
		RetourSerial_TODO = 1;
	}
	eteindreLedGauche();
}

