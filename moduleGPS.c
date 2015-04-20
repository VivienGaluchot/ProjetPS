#include <moduleGPS.h>

void initGPS(void){
	char* NMEA_OUTPUT = "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28<CR><LF>";

	iBuff0 = 0;
	iBuff1 = 0;
	useBuffer = 0;
	lengthWaitBuffer = 0;

	setENABLE_GPS(1);
	initTimer_A();
	setFuncTimer_A(&vidageBuffer);
	setTimer_A(1);

	setFuncRx0(&RxBuff0);
	connectGPS(1);
	
	//selection des trames $GPRMC
	//sendStrTX0(NMEA_OUTPUT);
}

void mettreEnAttente(char* data, int length){
	waitBuffer = data;
	lengthWaitBuffer = length;
}

void traiterDataGPS(){
	int i,j,chk;

	i = 0;
	while(i<lengthWaitBuffer){
		//avencée vers le premier $
		while(i<lengthWaitBuffer && waitBuffer[i]!='$') i++;
		// waitBuffer[i+1] contient le début des données de la trames

		//calcul du checksum
		j = i+1;
		chk = 0;
		while(j<lengthWaitBuffer && waitBuffer[j]!='*'){
			chk ^= waitBuffer[j];
			j++;
		}

		//trame valide ?
		if((j+3 < lengthWaitBuffer) && (chk == hexToInt(waitBuffer+j+1,2))){
			waitBuffer[j] = 0;
			debug_printf(waitBuffer+i+1);
			debug_printf("\n");
		}
		i++;
	}

	lengthWaitBuffer = 0;
}

int hexToInt(char *s, int length){
	int i,j,sum,hexPow;

	sum = 0;
	hexPow = 1;
	for(i=length-1;i>=0;i--){
		j = 0;
		while((j<16) && (s[i] != hexaTable[j]))
			j++;

		if(s[i] == hexaTable[j]){
			sum += j*hexPow;
			hexPow *= 16;
		}
		else{
			return 0; //caractère non reconnu
		}
	}

	return sum;
}

void RxBuff0(void){
	if(iBuff0 < BUFF_SIZE)
		buffer0[iBuff0++] = RXBUF0;
}

void RxBuff1(void){
	if(iBuff1 < BUFF_SIZE)
		buffer1[iBuff1++] = RXBUF0;
}

void vidageBuffer(void){
	if(useBuffer==0){
		useBuffer = 1;
		setFuncRx0(&RxBuff1);
		mettreEnAttente(buffer0,iBuff0+1);
		iBuff0 = 0;
	}
	else if(useBuffer==1){
		useBuffer = 0;
		setFuncRx0(&RxBuff0);
		mettreEnAttente(buffer1,iBuff1+1);
		iBuff1 = 0;
	}
}
