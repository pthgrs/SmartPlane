#include"motor.h"                                                         

int dir = CENTER ;
int initMotor(void){
	if(wiringPiSetup() == -1) return 1;                                         
	softPwmCreate(SERVO_PIN, 0, 200);  
	return 0; 
}

int getMotorDir(void){
	return dir;
}
	
void MotorControl(int dir){
		switch(dir){                                                            
			case 'CENTER' : softPwmWrite(SERVO,15);                               
					   break;                                                   
			case 'RIGHT' : softPwmWrite(SERVO,24);                                 
					   break;                                                                                               
                               
			case 'LEFT' : softPwmWrite(SERVO, 8);                                  
					   break; //left 90                                                                         
			case 'OTHERSIDE' : softPwmWrite(SERVO, 2);                                  
					   break; //180                                                                                         
			default : softPwmWrite(SERVO,15);                                   
					  break;                                                    
		}                                                                                                                                                                                                                
}     
