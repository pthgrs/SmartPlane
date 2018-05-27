#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>

int settingValues[3]={0}; 
int motorSetting[2]={0}; 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
//pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER; 

void * test_thread1(void *data){

	int option = 0;

	int flag = 0;
	char ch; 	

	while(1){

		pthread_mutex_lock(&mutex); 

		printf("input Options(0~2) : "); 
		scanf("%d", &option); 

		if(option == 0) {
			; //do nothing. 
		}
		else if(option == 1){

			//pthread_mutex_lock(&mutex); 

			printf("input settingValues : "); 
			for(int i=0; i<3; i++){
				scanf("%d", &settingValues[i]); 
			}

			//pthread_mutex_unlock(&mutex); 
		}
		else if(option == 2){

			//	pthread_mutex_lock(&mutex); 
			flag =1; 

			while(flag){
				motorSetting[0] =1; 
				printf("input motor dir : "); 
				scanf("%d", &motorSetting[1]);

				printf("change direction, continue? :"); 
				scanf("%c", &ch); 

				if(ch == 'y' || ch == 'Y'){
					flag = 1; 
				}
				else{
					flag =0; 
					motorSetting[0]= 0; 
				}	
				//	pthread_mutex_lock(&mutex); 
			}

		}
		pthread_mutex_unlock(&mutex); 
		sleep(5); 

	}
}


int main(){

	pthread_t thread1; 
	pthread_attr_t attr; 

	if(pthread_attr_init(&attr) != 0) return 1; 

	if(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)!=0) return -1; 


	int sensorValues[3]={0}; 


	pthread_create(&thread1, &attr, test_thread1, NULL); 

	while(1){

		printf("input sensorValues : "); 
		for(int i=0 ; i<3; i++){
			scanf("%d", &sensorValues[i]); 
		}


		pthread_mutex_lock(&mutex); 

		printf("*** setting Values : "); 
		for(int i=0; i<3; i++){
			printf(" %d ", settingValues[i]); 
		}

		printf("\n"); 

		for(int i=0; i<3; i++){
			if(sensorValues[i] < settingValues[i]){
				switch(i){
					case 0 : printf("temp error!!\n"); 
							 break;
					case 1 : printf("need water!!\n"); 
							 break;
					case 2 : printf("need light!!\n"); 
							 break; 
					default : printf("error!\n"); 
				}

			}
		}


		pthread_mutex_unlock(&mutex); 

		sleep(5); 
	}

	pthread_attr_destroy(&attr); 
	pthread_mutex_destroy(&mutex); 
	return 0; 
}
