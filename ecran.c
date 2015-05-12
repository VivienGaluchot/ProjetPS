#include <ecran.h>


char* getHeure(void);
char* getLatitude(void);
char* getLongitude(void);
char* getSatUsed(void);
char* getAltitude(void);
char* getSpeed(void);
char* getOrientation(void);
float getFloatOrientation(void);
char* getDate(void);
float getDistanceToDest(void);
char* getStrDistanceToDest(float distToDest);
float getCapToDest(float distToDest);
char* getStrCapToDest(float capToDest);
void setDestination(int);


void initScreen(void){
	etat = 1;
	menu1Item = 0;
	prevMenu1Item = 0;
	distanceTotale = 0;
	ecranStop = 0;

	pushedButton = 0;

	inMajScreen = 0;
	resetTODO();

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

void resetTODO(){
	if(inMajScreen)
		reset_TODO = 1;
	ClearScreen_TODO = 0;
	Menu1_TODO = 0;
	MajMenu1_TODO = 0;
	AffichageNavigation_TODO =0;
	MajNavigation_TODO = 0;
	MenuNavigation_TODO =0;
	MajMenuNavigation_TODO =0;
	AffichageEnregistrement_TODO = 0;
	MajEnregistrement_TODO = 0;
	AffichageBoussole_TODO = 0;
	MajBoussole_TODO = 0;
	PassageSerial_TODO = 0;
	RetourSerial_TODO = 0;

}

void majScreen(void){
	char ResOrbit[2];
	inMajScreen = 1;

	if(RetourSerial_TODO && !reset_TODO){
		connectUsbToScreen(0);
		setENABLE_GPS(1);
		connectGPS(1);
		connectScreen(1);
		RetourSerial_TODO = 0;
	}
	if(ClearScreen_TODO && !reset_TODO){
		clearScreen();
		ClearScreen_TODO = 0;
	}
	if(Menu1_TODO && !reset_TODO){
		menu1();
		Menu1_TODO = 0;
	}
	if(MajMenu1_TODO && !reset_TODO){
		majmenu1();
		MajMenu1_TODO = 0;
	}
	if(AffichageEnregistrement_TODO && !reset_TODO){
		affichageEnregistrement();
		AffichageEnregistrement_TODO = 0;
	}
	if(AffichageNavigation_TODO && !reset_TODO){
		affichageNavigation();
		AffichageNavigation_TODO =0;
	}
	if(AffichageBoussole_TODO && !reset_TODO){
		fondBoussole();
		AffichageBoussole_TODO =0;
	}
	if(MajBoussole_TODO && !reset_TODO){
		drawFilledCircle(63,63,45,noir);
		orbit(63,63,45,-getFloatOrientation(),ResOrbit);
		boussole(63,63,ResOrbit[0],ResOrbit[1],rouge);
		orbit(63,63,45,-getFloatOrientation()+180,ResOrbit);
		boussole(63,63,ResOrbit[0],ResOrbit[1],blanc);
	}
	if(PassageSerial_TODO && !reset_TODO){
		setENABLE_GPS(0);
		connectGPS(0);
		clearScreen();
		connectUsbToScreen(1);
		PassageSerial_TODO = 0;
	}
	if(MajEnregistrement_TODO && !reset_TODO){
		majaffichageEnregistrement();
	}
	if(MajNavigation_TODO && !reset_TODO){
		majaffichageNavigation();
	}
	if(MenuNavigation_TODO && !reset_TODO){
		menuNavigation();
		MenuNavigation_TODO =0;
	}
	if(MajMenuNavigation_TODO && !reset_TODO){
		majmenuNavigation();
		MajMenuNavigation_TODO =0;
	}
	if(!reset_TODO){
		pushedButton = 0;
	}

	inMajScreen = 0;
	reset_TODO = 0;
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

void fondBoussole(void){

	clearScreen();


	drawCircle(63,63,63,fondviolet);
	drawCircle(63,64,63,fondviolet);
	drawCircle(64,64,63,fondviolet);
	drawCircle(64,63,63,fondviolet);

	// drawTriangle(0,63,63,72,63,54,tourviolet);
	// drawTriangle(127,63,63,72,63,54,tourviolet);
	// drawTriangle(63,0,72,63,54,63,tourviolet);
	// drawTriangle(63,127,72,63,54,63,tourviolet);

	// drawTriangle(63-32,63-32,63+5,63-5,63-5,63+5,tourviolet);
	// drawTriangle(63+32,63+32,63+5,63-5,63-5,63+5,tourviolet);

	// drawTriangle(63+32,63-32,63+5,63+5,63-5,63-5,tourviolet);
	// drawTriangle(63-32,63+32,63+5,63+5,63-5,63-5,tourviolet);
	


	tailleText(2);
	printe("N",0,4,noir,tourviolet);
	// printe("E",4,8,noir,tourviolet);
	printe("S",7,4,noir,tourviolet);
	// printe("W",4,0,noir,tourviolet);
	tailleText(1);
}

void boussole(char x1,char y1,char x2, char y2,char *couleur){

	//drawFilledCircle(63,63,9,blanc);


	// drawLine( x1-2, y1-9,  x2, y2, couleur);
	// drawLine( x1-1, y1-9,  x2, y2, couleur);
	// drawLine( x1,   y1-9,  x2, y2, couleur);
	// drawLine( x1+1, y1-9,  x2, y2, couleur);
	// drawLine( x1+2, y1-9,  x2, y2, couleur);

	// drawLine( x1+9, y1+2,  x2, y2, couleur);
	// drawLine( x1+9, y1+1,  x2, y2, couleur);
	// drawLine( x1+9, y1,    x2, y2, couleur);
	// drawLine( x1+9, y1-1,  x2, y2, couleur);
	// drawLine( x1+9, y1-2,  x2, y2, couleur);

	// drawLine( x1-2, y1+9,  x2, y2, couleur);
	// drawLine( x1-1, y1+9,  x2, y2, couleur);
	// drawLine( x1,   y1+9,  x2, y2, couleur);
	// drawLine( x1+1, y1+9,  x2, y2, couleur);
	// drawLine( x1+2, y1+9,  x2, y2, couleur);

	// drawLine( x1-9, y1+2,  x2, y2, couleur);
	// drawLine( x1-9, y1+1,  x2, y2, couleur);
	// drawLine( x1-9, y1,    x2, y2, couleur);
	// drawLine( x1-9, y1-1,  x2, y2, couleur);
	// drawLine( x1-9, y1-2,  x2, y2, couleur);

	// drawLine( x1-3, y1-8,  x2, y2, couleur);
	// drawLine( x1-4, y1-8,  x2, y2, couleur);
	// drawLine( x1-5, y1-7,  x2, y2, couleur);
	// drawLine( x1-6, y1-7,  x2, y2, couleur);
	// drawLine( x1-7, y1-6,  x2, y2, couleur);
	// drawLine( x1-7, y1-5,  x2, y2, couleur);
	// drawLine( x1-8, y1-4,  x2, y2, couleur);
	// drawLine( x1-8, y1-3,  x2, y2, couleur);


	// drawLine( x1+3, y1-8,  x2, y2, couleur);
	// drawLine( x1+4, y1-8,  x2, y2, couleur);
	// drawLine( x1+5, y1-7,  x2, y2, couleur);
	// drawLine( x1+6, y1-7,  x2, y2, couleur);
	// drawLine( x1+7, y1-6,  x2, y2, couleur);
	// drawLine( x1+7, y1-5,  x2, y2, couleur);
	// drawLine( x1+8, y1-4,  x2, y2, couleur);
	// drawLine( x1+8, y1-3,  x2, y2, couleur);

	// drawLine( x1-3, y1+8,  x2, y2, couleur);
	// drawLine( x1-4, y1+8,  x2, y2, couleur);
	// drawLine( x1-5, y1+7,  x2, y2, couleur);
	// drawLine( x1-6, y1+7,  x2, y2, couleur);
	// drawLine( x1-7, y1+6,  x2, y2, couleur);
	// drawLine( x1-7, y1+5,  x2, y2, couleur);
	// drawLine( x1-8, y1+4,  x2, y2, couleur);
	// drawLine( x1-8, y1+3,  x2, y2, couleur);


	// drawLine( x1+3, y1+8,  x2, y2, couleur);
	// drawLine( x1+4, y1+8,  x2, y2, couleur);
	// drawLine( x1+5, y1+7,  x2, y2, couleur);
	// drawLine( x1+6, y1+7,  x2, y2, couleur);
	// drawLine( x1+7, y1+6,  x2, y2, couleur);
	// drawLine( x1+7, y1+5,  x2, y2, couleur);
	// drawLine( x1+8, y1+4,  x2, y2, couleur);
	// drawLine( x1+8, y1+3,  x2, y2, couleur);

	drawFilledCircle(63,63,4,rouge);
	drawLine( x1+1, y1, x2,y2,couleur);
	drawLine( x1-1, y1, x2,y2,couleur);
	drawLine( x1, y1+1, x2,y2,couleur);
	drawLine( x1, y1-1, x2,y2,couleur);

	// printe(getOrientation(),12,2,noir,tourorange);
}

void drawRectangle(char x1,char y1,char x2, char y2, char *couleur){

	sendCharTX1(0xff);
	sendCharTX1(0xcf);
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

void drawFilledRectangle(char x1,char y1,char x2, char y2, char *couleur){

	sendCharTX1(0xff);
	sendCharTX1(0xce);

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

void drawPolygon7(char x1,char x2,char x3,char x4,char x5,char x6,char x7,char y1,char y2,char y3,char y4,char y5,char y6,char y7, char *couleur ){

	sendCharTX1(0x00);
	sendCharTX1(0x04);
	sendCharTX1(0x00);
	sendCharTX1(0x07);

	sendCharTX1(0x00);
	sendCharTX1(x1);
	sendCharTX1(0x00);
	sendCharTX1(x2);
	sendCharTX1(0x00);
	sendCharTX1(x3);
	sendCharTX1(0x00);
	sendCharTX1(x4);
	sendCharTX1(0x00);
	sendCharTX1(x5);
	sendCharTX1(0x00);
	sendCharTX1(x6);
	sendCharTX1(0x00);
	sendCharTX1(x7);

	sendCharTX1(0x00);
	sendCharTX1(y1);
	sendCharTX1(0x00);
	sendCharTX1(y2);
	sendCharTX1(0x00);
	sendCharTX1(y3);
	sendCharTX1(0x00);
	sendCharTX1(y4);
	sendCharTX1(0x00);
	sendCharTX1(y5);
	sendCharTX1(0x00);
	sendCharTX1(y6);
	sendCharTX1(0x00);
	sendCharTX1(y7);

	sendCharTableTX1(couleur,2);
	waitACK_RX_1();

}

void arrow(char x, char y,float taille,float angle, char* couleur){
	char ResOrbit[2];
	char x1, x2, x3, x4, x5, x6, x7, y1, y2, y3, y4, y5, y6, y7;
	orbit(x,y,40*taille,angle,ResOrbit);
	x1=ResOrbit[0];
	y1=ResOrbit[1];
	orbit(x,y,sqrt(30*taille*30*taille+10*taille*10*taille),angle+71.565,ResOrbit);
	x2=ResOrbit[0];
	y2=ResOrbit[1];
	orbit(x,y,sqrt(10*taille*10*taille+10*taille*10*taille),angle+45,ResOrbit);
	x3=ResOrbit[0];
	y3=ResOrbit[1];
	orbit(x,y,sqrt(10*taille*10*taille+(-30*taille*(-30)*taille)),angle+161.565,ResOrbit);
	x4=ResOrbit[0];
	y4=ResOrbit[1];
	orbit(x,y,sqrt((-30*taille*(-30)*taille)+(-10*taille*(-10)*taille)),angle-161.565,ResOrbit);
	x5=ResOrbit[0];
	y5=ResOrbit[1];
	orbit(x,y,sqrt((-10*taille*(-10)*taille)+(10*taille*10*taille)),angle-45,ResOrbit);
	x6=ResOrbit[0];
	y6=ResOrbit[1];
	orbit(x,y,sqrt((-30*taille*(-30)*taille)+10*taille*10*taille),angle-71.565,ResOrbit);
	x7=ResOrbit[0];
	y7=ResOrbit[1];

//	drawPolygon7(x,x+30*taille,x+10*taille,x+10*taille,x-10*taille,x-10*taille,x-30*taille,y+40*taille,y+10*taille,y+10*taille,y-30*taille,y-30*taille,y+10*taille,y+10*taille,couleur);
	drawPolygon7(x1, x2, x3, x4, x5, x6, x7, y1, y2, y3, y4, y5, y6, y7,couleur);
}

void clearScreen(void){
	sendCharTX1(0xff);
	sendCharTX1(0xd7);
	waitACK_RX_1();
}

void moveTo(char x, char y){
	sendCharTX1(0xff);
	sendCharTX1(0xd6);
	sendCharTX1(0x00);
	sendCharTX1(x);
	sendCharTX1(0x00);
	sendCharTX1(y);
	waitACK_RX_1();
}


//Range dans ResOrbit x et y calculer
void orbit(int x, int y, int distance, float angle,char ResOrbit[]){
 	ResOrbit[0]=x+distance*cos((270+angle)*M_PI/180);
 	ResOrbit[1]=y+distance*sin((270+angle)*M_PI/180);
}  

void menu1(void){


	drawFilledRectangle(3,3,124,28,fondbleu);  
	drawFilledRectangle(3,35,124,60,fondorange);  
	drawFilledRectangle(3,67,124,92,fondviolet);  
	drawFilledRectangle(3,99,124,124,fondgris);  
	drawRectangle(3,3,124,28,tourbleu);  
	drawRectangle(3,35,124,60,tourorange);  
	drawRectangle(3,67,124,92,tourviolet);  
	drawRectangle(3,99,124,124,tourgris);  
	printe("Enregistrement",2,1,fondbleu,blanc);  
	printe("Navigation",6,1,fondorange,blanc);
	printe("Boussole",10,1,fondviolet,blanc);  
	printe("Serial",14,1,fondgris,blanc); 

}


void majmenu1(void){
	char color[2] ;
	int fixedItem = menu1Item; // menu1Item pouvant changer avec les interruptions au cours de cette fonction

	if(prevMenu1Item==0) { color[0] = fondbleu[0];	color[1] = fondbleu[1]; }
	else if(prevMenu1Item==1) { color[0] = fondorange[0];color[1] = fondorange[1]; }
	else if(prevMenu1Item==2) { color[0] = fondviolet[0];color[1] = fondviolet[1]; }
	else if(prevMenu1Item==3) { color[0] = fondgris[0];	color[1] = fondgris[1]; }

	drawRectangle(4,32*prevMenu1Item+4,123,32*prevMenu1Item+27,color); 
	drawRectangle(2,32*prevMenu1Item+2,125,32*prevMenu1Item+29,noir); 
	drawRectangle(1,32*prevMenu1Item+1,126,32*prevMenu1Item+30,noir); 
	drawRectangle(0,32*prevMenu1Item,127,32*prevMenu1Item+31,noir); 

	if(fixedItem==0) { color[0] = tourbleu[0];	color[1] = tourbleu[1]; }
	else if(fixedItem==1) { color[0] = tourorange[0];color[1] = tourorange[1]; }
	else if(fixedItem==2) { color[0] = tourviolet[0];color[1] = tourviolet[1]; }
	else if(fixedItem==3) { color[0] = tourgris[0];	color[1] = tourgris[1]; }

	drawRectangle(4,32*fixedItem+4,123,32*fixedItem+27,color); 
	drawRectangle(2,32*fixedItem+2,125,32*fixedItem+29,color); 
	drawRectangle(1,32*fixedItem+1,126,32*fixedItem+30,color); 
	drawRectangle(0,32*fixedItem,127,32*fixedItem+31,color); 

	prevMenu1Item = fixedItem;
}

void affichageEnregistrement(void){
	clearScreen();

	drawRectangle(0,0,127,127,bleuclair);
	drawRectangle(1,1,126,126,blert);
	drawRectangle(2,2,125,125,vert);
	drawRectangle(88,3,88,124,blert);
	drawRectangle(2,62,88,94,blert);
	drawRectangle(3,61,87,124,blert);
	drawRectangle(3,93,87,3,blert);

	drawFilledRectangle(83,3,124,18,noir);
	drawRectangle(81,3,125,19,blert);
	drawRectangle(80,2,126,20,blert);


	underline();
	printe("Position",1,1,noir,bleuclair);
	underline();
	printe("Altitude",8,1,noir,bleuclair);
	underline();
	printe("Vitesse",12,1,noir,bleuclair);

}

void majaffichageEnregistrement(void){
	printe(getLatitude(),3,1,noir,vert);
	drawCircle(31,25,1,vert);
	printe(getLongitude(),5,1,noir,vert);
	drawCircle(31,41,1,vert);

	printe(getAltitude(),10,2,noir,vert);
	printe(getSpeed(),14,2,noir,vert);
	printe(getHeure(),1,12,noir,vert);
}

void affichageNavigation(void){
	clearScreen();

	drawRectangle(0,0,127,127,rouge);
	drawRectangle(1,1,126,126,fondorange);
	drawRectangle(2,2,125,125,tourorange);
	drawRectangle(80,1,126,111,fondorange);
	drawRectangle(81,2,125,109,tourorange);
	drawRectangle(2,111,125,125,tourorange);
	drawRectangle(1,110,126,126,fondorange);

	underline();
	printe("Heure",1,12,noir,fondorange);
	underline();
	printe("nbSat",5,12,noir,fondorange);
	underline();
	printe("Speed",9,12,noir,fondorange);

	distanceTotale = getDistanceToDest();
}

void majaffichageNavigation(void){
	float progression, distToDest, capToDest;

	printe(getHeure(),3,12,noir,tourorange);
	printe(getSatUsed(),7,12,noir,tourorange);
	printe(getSpeed(),11,12,noir,tourorange);


	// calculs de distance et de cap, réutilisés plusieurs fois
	distToDest = getDistanceToDest();
	capToDest = getCapToDest(distToDest)-getFloatOrientation();

	printe(getStrDistanceToDest(distToDest),11,2,noir,tourorange);
	// printe(getStrCapToDest(capToDest),12,3,noir,tourorange);
	// printe(getOrientation(),13,2,noir,tourorange);
	
	

	drawFilledRectangle(4,4,79,79,noir);
	arrow(40,40,0.8,capToDest,fondorange);

	if(distanceTotale>distToDest)
		distanceTotale = distToDest;
	progression = ((distanceTotale-distToDest)/distanceTotale)*119+4;
	drawFilledRectangle(4,113,progression,123,rouge);
	drawFilledRectangle(progression+1,113,123,123,noir);
}

void menuNavigation(void){


	drawFilledRectangle(3,3,124,28,fondorange);  
	drawFilledRectangle(3,35,124,60,fondorange);  
	drawFilledRectangle(3,67,124,92,fondorange);  
	drawFilledRectangle(3,99,124,124,fondorange);  
	drawRectangle(3,3,124,28,tourorange);  
	drawRectangle(3,35,124,60,tourorange);  
	drawRectangle(3,67,124,92,tourorange);  
	drawRectangle(3,99,124,124,tourorange);  
	printe("Nantes",2,1,fondorange,blanc);  
	printe("Polytech",6,1,fondorange,blanc);
	printe("Genova",10,1,fondorange,blanc);  
	printe("Barcelone",14,1,fondorange,blanc); 

}

void majmenuNavigation(void){
	int fixedItem = menu1Item;

	drawRectangle(4,32*prevMenu1Item+4,123,32*prevMenu1Item+27,tourorange); 
	drawRectangle(2,32*prevMenu1Item+2,125,32*prevMenu1Item+29,noir); 
	drawRectangle(1,32*prevMenu1Item+1,126,32*prevMenu1Item+30,noir); 
	drawRectangle(0,32*prevMenu1Item,127,32*prevMenu1Item+31,noir); 

	drawRectangle(4,32*fixedItem+4,123,32*fixedItem+27,tourorange); 
	drawRectangle(2,32*fixedItem+2,125,32*fixedItem+29,tourorange); 
	drawRectangle(1,32*fixedItem+1,126,32*fixedItem+30,tourorange); 
	drawRectangle(0,32*fixedItem,127,32*fixedItem+31,tourorange); 

	prevMenu1Item = fixedItem;
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
	if(pushedButton != 1){
		resetTODO();
		if(ecranStop == 0){
			ClearScreen_TODO = 1;
			ecranStop = 1;
		}else{
			if(etat == 1){Menu1_TODO = 1;MajMenu1_TODO = 1;}
			else if(etat == 2){Menu1_TODO = 1;MajMenu1_TODO = 1;}
			else if(etat == 3){Menu1_TODO = 1;MajMenu1_TODO = 1;}
			else if(etat == 4){Menu1_TODO = 1;MajMenu1_TODO = 1;}
			else if(etat == 11){AffichageEnregistrement_TODO = 1;MajEnregistrement_TODO =1;}
			else if(etat == 21){MenuNavigation_TODO = 1;MajMenuNavigation_TODO = 1;}		
			else if(etat == 22){MenuNavigation_TODO = 1;MajMenuNavigation_TODO = 1;}
			else if(etat == 23){MenuNavigation_TODO = 1;MajMenuNavigation_TODO = 1;}
			else if(etat == 24){MenuNavigation_TODO = 1;MajMenuNavigation_TODO = 1;}
			else if(etat == 20){AffichageNavigation_TODO = 1;MajNavigation_TODO =1;}
			else if(etat == 31){AffichageBoussole_TODO = 1;MajBoussole_TODO = 1;}
			else if(etat == 41){PassageSerial_TODO = 1;}
			ecranStop =0;
		}
		
		pushedButton = 1;
	}
	eteindreLedMilieu();
}

void boutonHaut(void){
	if(pushedButton != 2){
		if(etat==1 || etat==2 || etat==3 || etat==4){
			resetTODO();
			menu1Item--;
			menu1Item = (menu1Item+8)%4;
			etat= menu1Item+1;
			MajMenu1_TODO = 1;
		}
		else if(etat==21 || etat==22 || etat==23 || etat==24 ){
			resetTODO();
			menu1Item--;
			menu1Item = (menu1Item+8)%4;
			etat= menu1Item+21;
			MajMenuNavigation_TODO = 1;
		}
		
		pushedButton = 2;
	}
	eteindreLedHaut();
}

void boutonDroit(void){
	if(pushedButton != 3){
		if (menu1Item==0 && etat == 1){
			resetTODO();
			AffichageEnregistrement_TODO = 1;
			MajEnregistrement_TODO =1;
			etat=11;
		}
		else if (menu1Item==1 && etat == 2){
			resetTODO();
			ClearScreen_TODO = 1;
			MenuNavigation_TODO = 1;
			menu1Item = 0;
			prevMenu1Item =0;

			MajMenuNavigation_TODO = 1;
			etat = 21;
		}
		else if (menu1Item==2 && etat == 3){
			resetTODO();
			AffichageBoussole_TODO = 1;
			MajBoussole_TODO = 1;
			etat=31;
		}
		else if (menu1Item==3 && etat == 4){
			resetTODO();
			PassageSerial_TODO = 1;
			etat=41;
		}
		else if (etat == 21){
			resetTODO();		
			AffichageNavigation_TODO = 1;
			MajNavigation_TODO =1;
			etat=20;
			setDestination(0);
		}
		else if (etat == 22){
			resetTODO();		
			AffichageNavigation_TODO = 1;
			MajNavigation_TODO =1;
			etat=20;
			setDestination(1);
		}
		else if (etat == 23){
			resetTODO();		
			AffichageNavigation_TODO = 1;
			MajNavigation_TODO =1;
			etat=20;
			setDestination(2);
		}
		else if (etat == 24){
			resetTODO();		
			AffichageNavigation_TODO = 1;
			MajNavigation_TODO =1;
			etat=20;
			setDestination(3);
		}

		
		pushedButton = 3;
	}
	eteindreLedDroite();
}

void boutonBas(void){
	if(pushedButton != 4){
		if(etat==1 || etat==2 || etat==3 || etat==4 ){
			resetTODO();
			menu1Item++;
			menu1Item = menu1Item%4;
			etat= menu1Item+1;
			MajMenu1_TODO = 1;
		}
		else if(etat==21 || etat==22 || etat==23 || etat==24 ){
			resetTODO();
			menu1Item++;
			menu1Item = menu1Item%4;
			etat= menu1Item+21;
			MajMenuNavigation_TODO = 1;
		}
		
		pushedButton = 4;
	}
	eteindreLedBas();
}

void boutonGauche(void){
	if(pushedButton != 5){
		resetTODO();
		if(etat == 11){
			ClearScreen_TODO = 1;
			etat = 1;
			Menu1_TODO = 1;
			MajMenu1_TODO = 1;
			MajEnregistrement_TODO =0;
		}
		else if(etat == 20){
			ClearScreen_TODO = 1;
			etat = menu1Item+21;
			MenuNavigation_TODO = 1;
			MajMenuNavigation_TODO = 1;
			MajNavigation_TODO =0;
		}
		else if(etat == 21 || etat == 22 || etat == 23 || etat == 24){
			ClearScreen_TODO = 1;
			menu1Item = 1;
			prevMenu1Item =1;
			etat = 2;
			Menu1_TODO = 1;
			MajMenu1_TODO = 1;
		}
		else if(etat == 31){
			ClearScreen_TODO = 1;
			etat = 3;
			Menu1_TODO = 1;
			MajMenu1_TODO = 1;
			MajBoussole_TODO = 0;
		}
		else if(etat == 41){
			ClearScreen_TODO = 1;
			etat = 4;
			Menu1_TODO = 1;
			MajMenu1_TODO = 1;
			RetourSerial_TODO = 1;
		}
		
		pushedButton = 5;
	}
	eteindreLedGauche();
}
