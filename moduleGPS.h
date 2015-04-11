#ifndef MODULE_GPS_H
#define MODULE_GPS_H

#include <msp430x16x.h>
#include <ports.h>
#include <uart.h>

void initGPS();

void traiterDataGPS(char* data);

#endif