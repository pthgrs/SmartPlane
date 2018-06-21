#ifndef __SMARTPLANT_H__
#define __SMARTPLANT_H__

#include <stdio.h>
#include <pthread.h>
#include "motor.h"
#include "sensor.h"
#include "server.h"

/****GPIO PIN NUMBER****/
#define CS_MCP3208 8	//Analog input CE0
#define SCLK 11		//Analog clock
#define MISO 9		//Analog Master Input Slave Output
#define MOSI 10		//Analog Master Output Slave Input


#define SENSOR_TERM 500	//몇ms마다 센서 측정할 것인지(1초 = 1000ms)
//스레드 mutex
pthread_mutex_t mutex_lock;

int client;

#endif
