#include <data.h>

int hexToInt(char *s, int length){
	int i,j,sum,hexPow,t;
	char error = 0;
	sum = 0;
	hexPow = 1;
	for(i=length-1;i>=0 && s[i]!=0 && !error;i--){
		j = 0;
		while((j<16) && (s[i] != hexaTable[j]))
			j++;
		if(s[i] == hexaTable[j]){
			t  = j*hexPow;
			sum = sum + t;
			hexPow *= 16;
		}
		else{
			error = 1; //caractère non reconnu
		}
	}
	if(error) sum = 0;
	return sum;
}

int decToInt(char *s, int length){
	int i,j,sum,decPow,t;
	char error = 0;
	sum = 0;
	decPow = 1;
	for(i=length-1;i>=0 && s[i]!=0 && !error;i--){
		j = s[i] - '0';
		if(0<=j && j<10){
			t  = j*decPow;
			sum = sum + t;
			decPow *= 10;
		}
		else{
			error = 1; //caractère non reconnu
		}
	}
	if(error) sum = 0;
	return sum;
}

int intToDec(int x, char *cible, int length){
	int i,j,t;
	i = 0;
	if(x == 0 && length>0){
		cible[0] = '0';
		if(length>1)
			cible[1] = 0;
		i = 1;
	}
	else{
		while(x!=0 && i<length){
			t = x/10;
			cible[i] = x - t*10 + '0';
			x = t;
			i++;
		}
		// inversion des caractères
		for(j=0;j<i/2;j++){
			t = cible[j];
			cible[j] = cible[i-j-1];
			cible[i-j-1] = t;
		}
		if(x!=0) {
			cible[0] = 0; //dépassement
			i = 0;
		}
		else cible[i] = 0;
	}
	return i;
}

float strToFloat(char *s, int length){
	int i,j,d;
	float sum;

	i = 0;
	while(s[i] != 0 && s[i] != '.' && i<length) i++;
	sum = decToInt(s,i);
	i++;
	j = i;
	d = 1;
	while(s[i] != 0 && i<length){
		i++;
		d = d * 10;
	}
	sum += (float)decToInt(s+j,i-j)/d;
	return sum;
}

int floatToStr(float x, char *cible, int length, int apresVirgule){
	int i,j,t;
	t = x; // partie entiere dans t
	i = intToDec(t,cible,length);
	if(i==0)
		cible[i++] = '0';
	if(i<length-2){
		j = 0;
		x = x-(float)t;
		while(j < apresVirgule){
			x = 10.0 * x;
			t = (int)x;
			j++;
		}
		t = intToDec(t,cible+i+1,length-(i+1));
		if(t>0){
			cible[i++] = '.';
			i+=t;
		}
	}

	return i;
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

int strCpy(char* cible, char*ref, int maxLen){
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

// lat : ddmm,mmmm
// lon : dddmm,mmmm
/* Exemple :
	coordConv(&A,"4851.0000","N","00221.0000","E");
	coordConv(&B,"4043.0000","N","07400.0000","W");
*/
void coordConv(gpsCoord *A, char* lat, char *NSind, char* lon, char*EWind){
	double t;

	//latitude en degrès
	t = decToInt(lat+5,4);
	t = t/10000;
	t = t + decToInt(lat+2,2);
	t = t/60;
	t = t + decToInt(lat,2);
	//latitude deg -> rad
	t = t*PI/180;
	//signe
	if(NSind[0] == 'N')
		A->lat = t;
	else if(NSind[0] == 'S')
		A->lat = -t;

	//longitude en degrès
	t = decToInt(lon+6,4);
	t = t/1000;
	t = t + decToInt(lon+3,2);
	t = t/60;
	t = t + decToInt(lon,3);
	//longitude deg -> rad
	t = t*PI/180;
	//signe
	if(EWind[0] == 'E')
		A->lon = t;
	else if(EWind[0] == 'W')
		A->lon = -t;
}

float distance(gpsCoord* A, gpsCoord* B){
	float res;
	res = sin(A->lat)*sin(B->lat) + cos(A->lat)*cos(B->lat)*cos(B->lon - A->lon);
	res = 60*acos(res);
	// rad -> deg
	res = (res)*180/PI;
	// miles -> km
	res = (res)*1.852;
	return res;
}

float cap(gpsCoord* A, gpsCoord* B, float distAB){
	float res1,res2,res;
	// calcul 1
	res1 = (cos(A->lat)*tan(B->lat)) / (sin(B->lon - A->lon)) - sin(A->lat)/(tan(B->lon - A->lon));
	res1 = atan(1/res1);
	// rad -> deg
	res1 = (res1)*180/PI;
	
	// calcul 2
	res2 = (cos(B->lat)*sin(B->lon - A->lon))/(sin(distAB/6371.0));
	res2 = asin(res2);
	// rad -> deg
	res2 = (res2)*180/PI;

	if(res1>=0 && res2>=0){
		// res entre 0 et 90
		res = (res1+res2) / 2;
	}
	else if(res1<=0 && res2>=0){
		// res entre 90 et 180
		res = 180 - (res2-res1)/2;
	}
	else if(res1>=0 && res2<=0){
		// res entre -180 et -90
		res = -180 - (res2-res1)/2;
	}
	else if(res1<=0 && res2<=0){
		// res entre -90 et 0
		res = (res1+res2) / 2;
	}

	while(res<0)
		res += 360;
	return res;
}
