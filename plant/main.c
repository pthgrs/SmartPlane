#include "smartPlant.h"

//extern int dir; //from motor.c
int dirs[4] = { CENTER, RIGHT, LEFT, OTHERSIDE } ; 

//초기화 함수
int init(void);

//서버 스레드
void* serverThread(void*);
//서버에서 받은 메시지를 파싱
void parsing(int, char*);
//사용자에게 알람보낼지 확인후 메시지 보냄
void checkAlarm(int);
//센서 스레드
void* sensorThread(void*);
//모터 스레드
void* motorThread(void*);

int main(void){
	init();
	int status;

	pthread_t thread[3];
	pthread_create(&thread[0],NULL,serverThread,NULL);
	pthread_create(&thread[1],NULL,sensorThread,NULL);
	pthread_create(&thread[2],NULL,motorThread,NULL);

	pthread_join(thread[0],(void**)&status);
	pthread_join(thread[1],(void**)&status);
	pthread_join(thread[2],(void**)&status); 
	return 0;
}

int init(void){
	//wiringPi 초기화
	if(wiringPiSetupGpio() == -1)
		return -1;
	//아날로그 센서 초기화
	if(initSensor(CS_MCP3208) == -1)
		return -2;
	
	//모터 초기화
	if(initMotor() == -1)
		return -3;
	//미연결 상태로
	client = -1;
	
	//뮤텍스 초기화(항상 0 리턴)
	pthread_mutex_init(&mutex_lock, NULL);
	isHot = 0;
	isCold = 0;
	isWet = 0;
	isDry = 0;
	isDark = 0; 
	isTooDark = 0; 

	return 0;
}
void* serverThread(void* data){
	while(1){
		client = init_server();

		while(1){
			//연결 끊기면 client = -1
			char* recv_message = read_server(client);
			if(recv_message == NULL){
				client = -1;
				disConnect();
				break;
			}
			else parsing(client, recv_message);
		}
	}
}

void* sensorThread(void* data){
	double temp, humi, light;
	while(1)
	{
		pthread_mutex_lock(&mutex_lock);
		temp = readSensorValue(TEMP_CHANNEL);
		#ifdef DEBUG
		printf("temp : %lf\n",temp);
		#endif
		setSensorValue(TEMP_CHANNEL,temp);
		pthread_mutex_unlock(&mutex_lock);
		
		pthread_mutex_lock(&mutex_lock);
		humi = readSensorValue(HUMIDITY_CHANNEL);
		setSensorValue(HUMIDITY_CHANNEL,humi);
		pthread_mutex_unlock(&mutex_lock);
		
		pthread_mutex_lock(&mutex_lock);
		light = readSensorValue(LIGHT_CHANNEL);
		setSensorValue(LIGHT_CHANNEL,light);
		pthread_mutex_unlock(&mutex_lock);
		#ifdef DEBUG
			printf("%d %d %d %d\n",
				isHot, isCold, isWet, isDry);
		#endif	
		checkSensors();
		checkAlarm(client);
		delay(SENSOR_TERM);		
	}	
}

void* motorThread(void* data){
	static int index = 0; 
	static int count = 0; 
	
	//사용자방향 입력에 대한 부분은 나중에 넣을게요. 
	while(1){
		pthread_mutex_lock(&mutex_lock);
		if(isDark && !isTooDark){ 
			index = (index+1)%4;
			count ++; 

			MotorControl(dirs[index]); 
			
			//4방향 다 돌았는데도 어두울 때. 
			if(count %4 == 0) isTooDark = 1; 
			
		}
		pthread_mutex_unlock(&mutex_lock);
		
		delay(SENSOR_TERM*3);	 //센서 3번 체크할 동안 멈춰있도록.
	}
	
}

void checkAlarm(int client){
	if(client != -1){
		if(isHot || isCold){
			double temp = getSensorValue(TEMP_CHANNEL);
			make_message(MODE_ALARM_TEMP,temp);
			write_server(client, SNDmessage);
		}
		if(isDry || isWet){
			double humi = getSensorValue(HUMIDITY_CHANNEL);
			make_message(MODE_ALARM_TEMP,humi);
			write_server(client, SNDmessage);
		}
		
		if(isTooDark){
			//화분 위치를 옮길 필요가 있을 때. 
			
			//경고메시지를 보내는 모드를 추가해주실 수 있을까요? 
			isTooDark = 0; 
		}
	}
}

void parsing(int client, char* message){
	int mode;
	char data[100];
#ifdef DEBUGM
	printf("message : %s\n",message);
#endif
	sscanf(message,"%d>%s",&mode,data);
#ifdef DEBUGM
	printf("mode : %d, data : %s\n",mode,data);
#endif
	if(mode == MODE_GET_DATA){
		double temp = getSensorValue(TEMP_CHANNEL);
		double humi = getSensorValue(HUMIDITY_CHANNEL);
		double light = getSensorValue(LIGHT_CHANNEL);
		make_message(MODE_SEND_DATA,temp,humi,light);
		write_server(client, SNDmessage);
	}else if(mode == MODE_SET_DATA1){
		int sensor;
		double sensorD, min, max;
		sscanf(data,"%lf,%lf,%lf<",&sensorD,&min,&max);
		sensor = sensorD;
#ifdef DEBUGM
		printf("sensor : %d, min : %lf max: %lf\n",
				sensor,min,max);
#endif
		pthread_mutex_lock(&mutex_lock);
		setSettingValue(sensor,min,max);
		getSettingValue();
		pthread_mutex_unlock(&mutex_lock);
	}
} 
