#ifndef MODULE_GPS_H
#define MODULE_GPS_H

#include <msp430x16x.h>
#include <ports.h>
#include <uart.h>
#include <interrupt.h>
#include <data.h>

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

char* getSatUsed(void);
char gps_SatUsed[2];
char gps_HDOP[16];

char* getAltitude(void);
char gps_Altitude[16];
char gps_AltUnit[16];

char* getSpeed(void);
char gps_SpeedOverGround[16];

char* getOrientation(void);
float getFloatOrientation(void);
char gps_CourseOverGround[16];

char* getDate(void);
char gps_Date[6];

// Validitée des données
int coordValid(void);
char gps_Status[1];
char gps_PosFixInd[1];

float getDistanceToDest(void);
char* getStrDistanceToDest(void);
float getCapToDest(void);
gpsCoord destCoord;

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

// Fonction interruptions
void RxBuff0(void);
void RxBuff1(void);
void vidageBuffer(void);

void test(void);

#endif
