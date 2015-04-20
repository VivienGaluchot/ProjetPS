#ifndef MODULE_GPS_H
#define MODULE_GPS_H

#include <msp430x16x.h>
#include <ports.h>
#include <uart.h>
#include <interrupt.h>

static char hexaTable[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void initGPS(void);
void traiterDataGPS(char* data, int lenght);

int hexToInt(char *s, int lenght);

#endif
