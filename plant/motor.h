#ifndef __MOTOR_H__
#define __MOTOR_H__

#include<wiringPi.h>
#include<stdio.h>
#include<softPwm.h>

#define SERVO 18 //GPIO 18 
#define PUMP_GPIO 5 //물 펌프 gpio

#define CENTER 15 //center.
#define RIGHT 24 //right 90
#define LEFT 8 //left 90
#define OTHERSIDE 2 //180도 회전

int initMotor(void);
int getMotorDir(void); 
void MotorControl(int dir);
void doWater(double amount);
double getWaterSettingValue();
int setWaterSettingValue(double);

double waterAmount;

#endif /* __MOTOR_H__ */ 
