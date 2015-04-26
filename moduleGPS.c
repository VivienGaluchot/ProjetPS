#include <moduleGPS.h>

void initGPS(void){
	strAnalyse(gps_UTCPos,"-",10);
	strAnalyse(gps_Latidude,"-",9);
	strAnalyse(gps_NSind,"-",1);
	strAnalyse(gps_Longitude,"-",10);
	strAnalyse(gps_EWind,"-",1);
	strAnalyse(gps_PosFixInd,"-",1);
	strAnalyse(gps_SatUsed,"-",2);
	strAnalyse(gps_HDOP,"-",16);
	strAnalyse(gps_Altitude,"-",16);
	strAnalyse(gps_AltUnit,"-",16);

	strAnalyse(gps_Status,"-",1);
	strAnalyse(gps_SpeedOverGround,"-",16);
	strAnalyse(gps_CourseOverGround,"-",16);
	strAnalyse(gps_Date,"-",6);

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
}

void mettreEnAttente(char* data, int length){
	waitBuffer = data;
	lengthWaitBuffer = length;
}

void traiterDataGPS(void){
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
			waitBuffer[j+3] = 0;
			debug_printf(waitBuffer+i);
			debug_printf("\n");
			if(!strCmp(waitBuffer+i,"$GPGGA")){
				i += 7;
				i += strAnalyse(gps_UTCPos, waitBuffer+i, 10) + 1;

				i += strAnalyse(gps_Latidude, waitBuffer+i, 9) + 1;
				i += strAnalyse(gps_NSind, waitBuffer+i, 1) + 1;
				i += strAnalyse(gps_Longitude, waitBuffer+i, 10) + 1;
				i += strAnalyse(gps_EWind, waitBuffer+i, 1) + 1;

				i += strAnalyse(gps_PosFixInd, waitBuffer+i, 1) + 1;
				i += strAnalyse(gps_SatUsed, waitBuffer+i, 2) + 1;
				i += strAnalyse(gps_HDOP, waitBuffer+i, 16) + 1;

				i += strAnalyse(gps_Altitude, waitBuffer+i, 16) + 1;
				i += strAnalyse(gps_AltUnit, waitBuffer+i, 16) + 1;
			}
			else if(!strCmp(waitBuffer+i,"$GPRMC")){
				i += 7;
				i += strAnalyse(gps_UTCPos, waitBuffer+i, 10) + 1;
				i += strAnalyse(gps_Status, waitBuffer+i, 1) + 1;

				i += strAnalyse(gps_Latidude, waitBuffer+i, 9) + 1;
				i += strAnalyse(gps_NSind, waitBuffer+i, 1) + 1;
				i += strAnalyse(gps_Longitude, waitBuffer+i, 10) + 1;
				i += strAnalyse(gps_EWind, waitBuffer+i, 1) + 1;

				i += strAnalyse(gps_SpeedOverGround, waitBuffer+i, 16) + 1;
				i += strAnalyse(gps_CourseOverGround, waitBuffer+i, 16) + 1;
				i += strAnalyse(gps_Date, waitBuffer+i, 6) + 1;
			}
		}
		i++;
	}

	lengthWaitBuffer = 0;
}

int hexToInt(char *s, int length){
	int i,j,sum,hexPow,t;

	sum = 0;
	hexPow = 1;
	for(i=length-1;i>=0;i--){
		j = 0;
		while((j<16) && (s[i] != hexaTable[j]))
			j++;

		if(s[i] == hexaTable[j]){
			t  = j*hexPow;
			sum = sum + t;
			hexPow *= 16;
		}
		else{
			return 0; //caractère non reconnu
		}
	}

	return sum;
}

char strCmp(char* cible, char* ref){
	int i;
	char res;
	res = 0;
	i = 0;
	while(cible[i]!=0 && cible[i]!=',' && cible[i]!='*' && ref[i]!=0 && ref[i]!=',' && ref[i]!='*' && res==0){
		res = cible[i] - ref[i];
		i++;
	}

	return res;
}

int strAnalyse(char* cible, char*ref, int maxLen){
	int i;
	i = 0;
	if(ref[i]==0 || ref[i]==',' || ref[i]=='*'){
		cible[i] = '-';
	}
	else{
		while(ref[i]!=0 && ref[i]!=',' && ref[i]!='*' && i<maxLen){
			cible[i] = ref[i];
			i++;
		}
		if(i<maxLen)
			cible[i] = 0;
	}

	return i;
}

float distance(float lat1, float lon1, float lat2, float lon2){
	return 1.0;
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
