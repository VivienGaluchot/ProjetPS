#ifndef DATA_H
#define DATA_H

#include <math.h>

#define PI (3.141592653589793)
#define RAYON_TERRE ()

// Point de coordonées GPS (en rad)
typedef struct this {
	double lat;
	double lon;
} gpsCoord;

// Convertions
static char hexaTable[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
int hexToInt(char *s, int length);
int decToInt(char *s, int length);
int intToDec(int x, char *cible, int length);

float strToFloat(char *s, int length);
int floatToStr(float x, char *cible, int length, int apresVirgule); //return le nombre de char utilisés

// Str opérations
char strCmp(char* cible, char* ref);
int strCpy(char* cible, char*ref, int maxLen); //return le nombre de char copié

// lat : "ddmm,mmmm"
// lon : "dddmm,mmmm"
void coordConv(gpsCoord *A, char* lat, char *NSind, char* lon, char*EWind);
float distance(gpsCoord* A, gpsCoord* B);
float cap(gpsCoord* A, gpsCoord* B, float distAB);

#endif
