#ifdef __SENSOR_H__
#define __SENSOR_H__

#include<stdio.h>
#include<WiringPi.h>
#include<pigpiod_if2.h>

#define BAUD_RATE 1000000
#define LOOP 1000

int initSensor(void);
int getHumidity(unsigned char channel);


#endif /* __SENSOR_H__
