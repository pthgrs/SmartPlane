/***추가해야할 부분****
 * 조도관련 센서 : CheckSensors(), mcp_to_light()
 */



//센서 헤더 중복되지 않도록 처리
#ifndef __SENSOR_H__
#define __SENSOR_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

//wiringPi관련
#include <wiringPi.h>
#include <wiringPiSPI.h>

/*****상수 설정****/
#define SPI_CHANNEL 0		//CE0 CHANNEL
#define SPI_SPEED 1000000	//샘플링 횟수(1Mhz)

#define TEMP_CHANNEL 0		//온도의 mcp3208 channel
#define HUMIDITY_CHANNEL 1 	//습도의 mcp3208 channel
#define LIGHT_CHANNEL 2		//조도의 mcp3208 channel

#define VOLTAGE 3.3		//mcp3208 전압
#define MCP3208_MAX 4095	//mcp3208 결과값 최대값

#define HUMIDITY_MARGIN 300	//습도 센서 오차조정

/****센서값 저장****/
//센서 값 저장 구조체
//[0] : 최소값, 실시간 값 / [1] : 최대값
typedef struct _Sensor{
	double temp[2];
	double humidity[2];
	double light[2];
}Sensor;

Sensor settingValue; //유저 설정 값
Sensor realTimeValue;//실시간 센서 값

/****센서 체크****/
//온도관련
int isCold;
int isHot;
//습도관련
int isDry;
int isWet;
int isWater;
clock_t waterTime;
clock_t WATER_DIFF;
//조도관련
int isDark; 
int isTooDark; //4방향 다 돌았는데도 어두울 때. 
int direction;	//0~4(동,서,남,북)

//센서 관련 초기화
int initSensor(unsigned char gpio);

//유저 설정값을 파일에서 읽어서 구조체에  저장
int getSettingValue();
//유저 설정값을 파일에 저장
int setSettingValue(unsigned char channel, double min, double max);

//채널에 맞는 센서값을 센서에서 읽음 
double readSensorValue(unsigned char channel);
//채널에 맞는 센서값 저장
int setSensorValue(unsigned char channel, double value);
//채널에 맞는 센서값 불러옴 
double getSensorValue(unsigned char channel);

void checkSensors(void);

#endif /* __SENSOR_H__*/
