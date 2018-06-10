#ifndef __MOTOR_H__
#define __MOTOR_H__

#include<wiringPi.h>
#include<stdio.h>
#include<softPwm.h>

#define SERVO 1 //GPIO 18 

#define CENTER 15 //center.
#define RIGHT 24 //right 90
#define LEFT 8 //left 90
#define OTHERSIDE 2 //180도 회전

int initMotor(void);
int getMotorDir(void); 
void MotorControl(int dir); 

#endif /* __MOTOR_H__ */ 
