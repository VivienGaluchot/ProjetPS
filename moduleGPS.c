#include <moduleGPS.h>

void initGPS(void){
	strCpy(gps_UTCPos,"-",10);
	strCpy(gps_Latitude,"-",9);
	strCpy(gps_NSind,"-",1);
	strCpy(gps_Longitude,"-",10);
	strCpy(gps_EWind,"-",1);
	strCpy(gps_PosFixInd,"-",1);
	strCpy(gps_SatUsed,"-",2);
	strCpy(gps_HDOP,"-",16);
	strCpy(gps_Altitude,"-",16);
	strCpy(gps_AltUnit,"-",16);

	strCpy(gps_Status,"-",1);
	strCpy(gps_SpeedOverGround,"-",16);
	strCpy(gps_CourseOverGround,"-",16);
	strCpy(gps_Date,"-",6);

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
			if(!strCmp(waitBuffer+i,"$GPGGA")){
				i += 7;
				i += strCpy(gps_UTCPos, waitBuffer+i, 10) + 1;

				i += strCpy(gps_Latitude, waitBuffer+i, 9) + 1;
				i += strCpy(gps_NSind, waitBuffer+i, 1) + 1;
				i += strCpy(gps_Longitude, waitBuffer+i, 10) + 1;
				i += strCpy(gps_EWind, waitBuffer+i, 1) + 1;

				i += strCpy(gps_PosFixInd, waitBuffer+i, 1) + 1;
				i += strCpy(gps_SatUsed, waitBuffer+i, 2) + 1;
				i += strCpy(gps_HDOP, waitBuffer+i, 16) + 1;

				i += strCpy(gps_Altitude, waitBuffer+i, 16) + 1;
				i += strCpy(gps_AltUnit, waitBuffer+i, 16) + 1;
			}
			else if(!strCmp(waitBuffer+i,"$GPRMC")){
				i += 7;
				i += strCpy(gps_UTCPos, waitBuffer+i, 10) + 1;
				i += strCpy(gps_Status, waitBuffer+i, 1) + 1;

				i += strCpy(gps_Latitude, waitBuffer+i, 9) + 1;
				i += strCpy(gps_NSind, waitBuffer+i, 1) + 1;
				i += strCpy(gps_Longitude, waitBuffer+i, 10) + 1;
				i += strCpy(gps_EWind, waitBuffer+i, 1) + 1;

				i += strCpy(gps_SpeedOverGround, waitBuffer+i, 16) + 1;
				i += strCpy(gps_CourseOverGround, waitBuffer+i, 16) + 1;
				i += strCpy(gps_Date, waitBuffer+i, 6) + 1;
			}
		}
		i++;
	}

	lengthWaitBuffer = 0;
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

char* getHeure(void){
	temp[0] = gps_UTCPos[0];
	temp[1] = gps_UTCPos[1];
	if((gps_UTCPos[5] - '0')%2)
		temp[2] = ':';
	else
		temp[2] = ' ';
	temp[3] = gps_UTCPos[2];
	temp[4] = gps_UTCPos[3];
	temp[5] = 0;
	return temp;
}

char* getLatitude(void){
	int tconv;
	char tchar[2];
	if(gps_Latitude[0] != '-'){
		temp[0] = '0';
		temp[1] = gps_Latitude[0];
		temp[2] = gps_Latitude[1];
		temp[3] = ' ';
		temp[4] = gps_Latitude[2];
		temp[5] = gps_Latitude[3];
		temp[6] = '\'';
		tconv = decToInt(gps_Latitude+5,4) * 0.6 / 100.0;
		intToDec(tconv,tchar,2);
		temp[7] = tchar[0];
		temp[8] = tchar[1];
		temp[9] = '\"';
		temp[10] = gps_NSind[0];
		temp[11] = 0;
	}
	else{
		temp[0] = '-';
		temp[1] = 0;
	}
	return temp;
}

char* getLongitude(void){
	int tconv;
	char tchar[2];
	temp[0] = gps_Longitude[0];
	temp[1] = gps_Longitude[1];
	temp[2] = gps_Longitude[2];
	temp[3] = ' ';
	temp[4] = gps_Longitude[3];
	temp[5] = gps_Longitude[4];
	temp[6] = '\'';
	tconv = decToInt(gps_Longitude+6,4) * 0.6 / 100.0;
	intToDec(tconv,tchar,2);
	temp[7] = tchar[0];
	temp[8] = tchar[1];
	temp[9] = '\"';
	temp[10] = gps_EWind[0];
	temp[11] = 0;
	return temp;
}

char* getSatUsed(void){
	temp[0] = gps_SatUsed[0];
	temp[1] = gps_SatUsed[1];
	temp[2] = 0;
	return temp;
}

char* getAltitude(void){
	int i = 0;
	int j = 0;
	while(gps_Altitude[i]!=0 && i<16){
		temp[i] = gps_Altitude[i];
		i++;
	}
	temp[i] = ' ';
	i++;
	while(gps_AltUnit[j]!=0 && i<16){
		temp[i] = gps_AltUnit[j];
		i++;
		j++;
	}
	while(i<10) temp[i++] = ' ';
	temp[i] = 0;
	return temp;
}

char* getSpeed(void){
	int i = 0;
	float tconv;
	tconv = strToFloat(gps_SpeedOverGround,16);
	tconv = tconv  * 1.852;
	i = floatToStr(tconv,temp+i,10,1);
	temp[i++] = ' ';
	temp[i++] = 'k';
	temp[i++] = 'm';
	temp[i++] = '/';
	temp[i++] = 'h';
	while(i<10) temp[i++] = ' ';
	temp[i] = 0;
	return temp;
}

char* getOrientation(void){
	int i = 0;
	while(gps_CourseOverGround[i] && i<16){
		temp[i] = gps_CourseOverGround[i];
		i++;
	}
	temp[i] = 0;
	return temp;
}

float getFloatOrientation(void){
	float result;
	result = strToFloat(gps_CourseOverGround,16);
	return result;
}

char* getDate(void){
	return gps_Date;
}

float getDistanceToDest(void){
	gpsCoord current;
	coordConv(&current,gps_Latitude,gps_NSind,gps_Longitude,gps_EWind);
	coordConv(&destCoord,"4851.0000","N","00221.0000","E"); // paris comme dest pour test
	return distance(&current,&destCoord);
}

char* getStrDistanceToDest(void){
	int i = 0;
	i += floatToStr(getDistanceToDest(),temp,10,1);
	temp[i++] = ' ';
	temp[i++] = 'k';
	temp[i++] = 'm';
	temp[i] = 0;
	return temp;
}

float getCapToDest(void){
	gpsCoord current;
	coordConv(&current,gps_Latitude,gps_NSind,gps_Longitude,gps_EWind);
	coordConv(&destCoord,"4851.0000","N","00221.0000","E"); // paris comme dest pour test
	return cap(&current,&destCoord);
}
