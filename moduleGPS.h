#ifndef MODULE_GPS_H
#define MODULE_GPS_H

#include <msp430x16x.h>
#include <ports.h>
#include <uart.h>
#include <interrupt.h>
#include <math.h>

typedef struct this {
	double lat;
	double lon;
} gpsCoord;

#define PI (3.141592653589793)

void initGPS(void);

// Données
char* getHeure(void);
char gps_UTCPos[10];
char* getLatitude(void);
char gps_Latitude[9];
char gps_NSind[1];
char* getLongitude(void);
char gps_Longitude[10];
char gps_EWind[1];
char gps_PosFixInd[1];
char* getSatUsed(void);
char gps_SatUsed[2];
char gps_HDOP[16];
char* getAltitude(void);
char gps_Altitude[16];
char gps_AltUnit[16];

char gps_Status[1];
char* getSpeed(void);
char gps_SpeedOverGround[16];
char* getCourseOverGroud[16];
char gps_CourseOverGround[16];
char* getDate(void);
char gps_Date[6];

char temp[17];

// Buffer de reception GPS
#define BUFF_SIZE 512
static char buffer0[BUFF_SIZE];
static char buffer1[BUFF_SIZE];
static int iBuff0 = 0;
static int iBuff1 = 0;
static char useBuffer = 0;

static char* waitBuffer;
static int lengthWaitBuffer;
void mettreEnAttente(char* data, int length);
void traiterDataGPS(void);

static char hexaTable[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
int hexToInt(char *s, int length);

int decToInt(char *s, int length);

char strCmp(char* cible, char* ref);
int strAnalyse(char* cible, char*ref, int maxLen); //return le nombre de char copié

// lat : ddmm,mmmm
// lon : dddmm,mmmm
void coordConv(gpsCoord *A, char* lat, char *NSind, char* lon, char*EWind);
float distance(gpsCoord A, gpsCoord B);
float cap(gpsCoord A, gpsCoord B);

// Fonction interruptions
void RxBuff0(void);
void RxBuff1(void);
void vidageBuffer(void);

void test(void);

#endif
