#ifndef MODULE_GPS_H
#define MODULE_GPS_H

#include <msp430x16x.h>
#include <ports.h>
#include <uart.h>

static char bufferUART0[400];
static char bufferUART1[400];
static int iBuff0 = 0;
static int iBuff1 = 0;
static int selBuff = 0;

void initUartGPS();
void activerUartGPS();
//void usart0_rx();

void traiterDataGPS();

#endif