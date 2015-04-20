#include <moduleGPS.h>

void initGPS(void){
	char* NMEA_OUTPUT = "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28<CR><LF>";
	// char* NMEA_OUTPUT_infos = "PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
	// int chk = checksum(NMEA_OUTPUT_infos);

	setENABLE_GPS(1);
	initTimer_A();
	setFuncTimer_A(&vidageBuffer);
	setTimer_A(1);

	connectGPS(1);
	
	//selection des trames $GPRMC
	//sendStrTX0(NMEA_OUTPUT);
}

void traiterDataGPS(char* data, int lenght){
	int i,j,chk;
	
	i = 0;
	while(i<lenght){
		//avencée vers le premier $
		while(i<lenght && data[i]!='$') i++;
		// data[i+1] contient le début des données de la trames

		//calcul du checksum
		j = i+1;
		chk = 0;
		while(j<lenght && data[j]!='*'){
			chk ^= data[j];
			j++;
		}

		//trame valide ?
		if((j+3 < lenght) && (chk == hexToInt(data+j+1,2))){
			data[j] = 0;
			debug_printf(data+i+1);
		}
		else{
			debug_printf("invalide");
		}
		debug_printf("\n");
		i++;
	}
}

int hexToInt(char *s, int lenght){
	int i,j,sum,hexPow;

	sum = 0;
	hexPow = 1;
	for(i=lenght-1;i>=0;i--){
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