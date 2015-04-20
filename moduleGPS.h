#ifndef MODULE_GPS_H
#define MODULE_GPS_H

#include <msp430x16x.h>
#include <ports.h>
#include <uart.h>
#include <interrupt.h>

void initGPS(void);

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
void traiterDataGPS();

int hexToInt(char *s, int length);
// Utilisé pour la conversion hexToInt
static char hexaTable[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

// Fonction interruptions
void RxBuff0(void);
void RxBuff1(void);
void vidageBuffer(void);

#endif
