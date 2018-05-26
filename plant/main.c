#include<stdio.h>
#include"motor.h"
#include"sensor.h"


//채널번호 설정.
#define LIGHT_CH 1
#define HUMIDITY_CH 2 


extern int pi;  //from sensor.c
extern int spi; //from sensor.c
extern int dir; //from motor.c

int main(void){
  double settingValues[3]; // 사용자가 지정한 값들 저장할 배열. (조도, 토양습도, 온도 순으로)
  double sensorValues[3]; // 센서로부터 읽어온 값을 저장할 배열. (조도, 토양습도, 온도 순으로)
  
  int dirs[4] = { CENTER, RIGHT, LEFT, OTHERSIDE } ; 
  
  int checkValue[2]={0}; 
  int controlFlag = 0; //사용자 입력 여부.
  int controlDir ; 
  
  //사용자로부터 값 받아오기. 
  /*  
  **(작성필요)
  */ 
  
  //init
  if( initSensor(void)!=0) printf("sensor error\n"); 
  if( initMotor(void) !=0) printf("motor error\n");
  
  while(1){
    sensorValues[0] = getSensorValue(LIGHT_CH);
    sensorValues[1] = getSensorValue(HUMIDITY_CH);
    //sensorValues[2] = /**(작성필요)**/   ; //온도 읽어와서 저장하기. 
    
    //사용자에게 현재까지 읽어온 센서값들을 전송해준다. 
    /*
    **(작성필요)
    */
    
	//사용자의 입력이 있을 경우.(모터조작)
	controlFlag = 1; 
	//입력에 따라서 모터를 동작시킬 것. 
	/*
	**(작성필요) : 사용자의 모터 방향 입력. (0~3?)
	*/ 
	//controlDir = ;
	MotorControl(dirs[controlDir]); //모터 조작. 
	//조작을 종료할 경우엔 다시 정상적으로 조도값을 확인하도록 한다. 
	controlFlag = 0; 
	
	
    if(controlFlag==0 && (sensorValue[0] < settingValues[0])){ //모터를 돌려줘야하는 상황이다. 조도값이 설정값보다 낮은 경우. 
      for(int i=0; i<4; i++){
        MotorControl(dirs[i]); 
        time_sleep(1); 
        sensorValues[0] = getSensorValue(LIGHT_CH); //1초 쉬고 다시 측정한다. 
        if(sensorValue[0] > settingValues[0]){ //정상적으로 설정된 경우. 
          checkValue[0] = 1; 
          break; 
         }
       }
       if(checkValue[0]){ //한바퀴 돌았음에도 개선되지 않은 경우
            //사용자에게 에러메시지를 전송한다.  : 화분의 위치를 옮길 필요가 있어보입니다. 
            /*
            **(작성필요)
            */ 
       }
       checkValue[0] = 0; 
    }
    
    if(sensorValue[1] < settingValues[1]){ //펌프를 가동해야하는 상황이다. 토양습도가 설정값보다 낮은 경우. 
      //펌프 가동 
      /*
      **(작성필요)
      */ 
      time_sleep(1); 
      sensorValue[1] = getSensorValue(HUMIDITY_CH); 
      if(sensorValue[1]< settingValues[1]){
         //사용자에게 에러메시지를 전송한다. : 물통에 물이 없는 것같습니다. 
         /*
         **(작성필요)
         */ 
      }
      
	  time_sleep(3);//센서 측정간격. 
	  //일단 임의로 3초로 해두었다. 
	  //사용자 입력과 센서 측정을 별개로 하려면 따로 코드 추가가 필요할 것으로 보임. 
    }
	
	closeSensor(void);
	return 0; 
}
    
    
