#ifdef __SENSOR_H__
#define __SENSOR_H__

#include<stdio.h>
#include<WiringPi.h>
#include<pigpiod_if2.h>

#define BAUD_RATE 1000000
#define LOOP 1000

int initSensor(void);
uint16_t getSensorValue(unsigned char channel);
void closeSensor(void); 

#endif /* __SENSOR_H__
