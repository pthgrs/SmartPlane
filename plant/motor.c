#include"motor.h"                                                         

int dir = CENTER ;

int initMotor(void){
	//if(wiringPiSetupGpio() == -1) return -1;
	pinMode(PUMP_GPIO,OUTPUT);                                         
	softPwmCreate(SERVO, 0, 200);  
	softPwmWrite(SERVO,15); //방향 초기화. 
	waterAmount = getWaterSettingValue();
	printf("amountSetting : %lf\n",waterAmount);
	return 0; 
}

int getMotorDir(void){
	return dir;
}
	
void MotorControl(int dir){
		switch(dir){                                                            
			case CENTER : softPwmWrite(SERVO,15);                               
					   break;                                                   
			case RIGHT : softPwmWrite(SERVO,24);                                 
					   break;                                                                                               
                               
			case LEFT : softPwmWrite(SERVO, 8);                                  
					   break; //left 90                                                                         
//			case OTHERSIDE : softPwmWrite(SERVO, 2);                                  
//					   break; //180                                                                                         
			default : softPwmWrite(SERVO,15);                                   
					  break;                                                    
		}                                                                                                                                                                                                                
}
//amount = ml
//초당 25ml 이동
void doWater(double amount){
	double sec = amount / 12;
	printf("doWater : %lf %lf\n",sec,amount);
	digitalWrite(PUMP_GPIO,1);
	delay(sec*1000);
	digitalWrite(PUMP_GPIO,0);
	delay(1000);
}

double getWaterSettingValue(){
	FILE *waterFile;
	double amount;
	waterFile = fopen("/home/pi/SmartPlant/Setting/settingW","r");
	if(waterFile == NULL){
		fprintf(stderr,"fail to Open water setting\n");
		return -1;
	}
	fscanf(waterFile, "%lf",&amount);
	printf("%lf\n",amount);
	fclose(waterFile);
	return amount;
}
int setWaterSettingValue(double amount){
	FILE *waterFile;
	waterFile = fopen("/home/pi/SmartPlant/Setting/settingW","w");
	if(waterFile == NULL){
		fprintf(stderr,"fail to Open water setting\n");
		return -1;
	}
	fprintf(waterFile, "%lf",&waterAmount);
	fclose(waterFile);
	return 0;
}
	
	
