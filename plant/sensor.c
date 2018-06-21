#include"sensor.h"

const char* filePath = "/home/pi/SmartPlant/Setting/setting";

//센서 값 읽음(return 값 : mv)
int read_mcp3208_adc(unsigned char);

//읽은 센서 값을 온도로 변환
double mcp_to_temperature(int);
//읽은 센서 값을 습도로 변환
double mcp_to_humidity(int);
//읽은 센서 값을 조도로 변환
double mcp_to_light(int);

//for debug
void printSensor(void);

//mcp3208 CE0
unsigned char CS_MCP3208;
/*TEST
int main(void){
	double temp, humi;
	if(wiringPiSetupGpio() == -1) return 1;
	initSensor(8);
	while(1){
		temp = readSensorValue(TEMP_CHANNEL);
		setSensorValue(TEMP_CHANNEL,temp);
		humi = readSensorValue(HUMIDITY_CHANNEL);
		printf("humi1 : %f\n",humi);
	        setSensorValue(HUMIDITY_CHANNEL,humi);
		printSensor();
		sleep(2);
	}
	return 0;

}
*/
//디버그용 print문
void printSensor(){
	double setTempMin = settingValue.temp[0];
	double setTempMax = settingValue.temp[1];
	double setHumiMin = settingValue.humidity[0];
	double setHumiMax = settingValue.humidity[1];
	double setLightMin = settingValue.light[0];
	double setLightMax = settingValue.light[1];
	
	printf("설정\n");
	printf("%.2fC ~ %.2fC %.2f%% ~ %.2f%% %.2flux ~ %.2flux\n",
			setTempMin,setTempMax,setHumiMin,setHumiMax,
			setLightMin, setLightMax);
	
	double temp = realTimeValue.temp[0];
	double humi = realTimeValue.humidity[0];
	printf("실시간\n");
	printf("온도 : %.2fC, 습도 : %.2f%%\n",temp,humi);
}

int initSensor(unsigned char gpio){
	if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1)
		return -1;
	if(getSettingValue() == -1)
		return -1;
	CS_MCP3208 = gpio;
	pinMode(CS_MCP3208, OUTPUT);
	isHot = 0; isCold=0;
	isDry=0; isWet = 0; isWater = 0;
	delay(1000);
	WATER_DIFF = 10  * 60 * 2100;
        isDark = 0;  isTooDark=0;	
	return 0;
}

int getSettingValue(){
	FILE *settingFile;
	//파일 읽기 모드로 열음
	//파일은 반드시 존재해야함
	settingFile = fopen(filePath,"r");
	if (settingFile == NULL)
		return -1;
		
	fscanf(settingFile,"%lf %lf %lf %lf %lf %lf",
			&settingValue.temp[0],&settingValue.temp[1],
		       	&settingValue.humidity[0],&settingValue.humidity[1],
		       	&settingValue.light[0],&settingValue.light[1]);
#ifdef DEBUGM
			printf("SETTING\n");
			printf("%.2lf %.2lf %.2lf %.2lf %.2lf %.2lf\n",
			settingValue.temp[0],settingValue.temp[1],
		       	settingValue.humidity[0],settingValue.humidity[1],
		       	settingValue.light[0],settingValue.light[1]);

#endif
	fclose(settingFile);
	return 0;
}

int setSettingValue(unsigned char channel, double min, double max){
	FILE *settingFile;
	settingFile = fopen(filePath,"w");
	switch(channel){
		case TEMP_CHANNEL:
			settingValue.temp[0] = min;
			settingValue.temp[1] = max;
			fprintf(settingFile,"%.2lf %.2lf %.2lf %.2lf %.2lf %.2lf",
			settingValue.temp[0],settingValue.temp[1],
		       	settingValue.humidity[0],settingValue.humidity[1],
		       	settingValue.light[0],settingValue.light[1]);
#ifdef DEBUGM
			printf("min : %lf, max : %lf\n",min,max);
			printf("%.2lf %.2lf %.2lf %.2lf %.2lf %.2lf\n",
			settingValue.temp[0],settingValue.temp[1],
		       	settingValue.humidity[0],settingValue.humidity[1],
		       	settingValue.light[0],settingValue.light[1]);

#endif
			break;
		case HUMIDITY_CHANNEL:
			settingValue.humidity[0] = min;
			settingValue.humidity[1] = max;
			fprintf(settingFile,"%.2lf %.2lf %.2lf %.2lf %.2lf %.2lf",
			settingValue.temp[0],settingValue.temp[1],
		       	settingValue.humidity[0],settingValue.humidity[1],
		       	settingValue.light[0],settingValue.light[1]);
			break;
		case LIGHT_CHANNEL:
			settingValue.light[0] = min;
			settingValue.light[1] = max;
			fprintf(settingFile,"%.2lf %.2lf %.2lf %.2lf %.2lf %.2lf",
			settingValue.temp[0],settingValue.temp[1],
		       	settingValue.humidity[0],settingValue.humidity[1],
		       	settingValue.light[0],settingValue.light[1]);
			break;
		defualt : 
			return -1;
		       	break;
	}
	fclose(settingFile);
	return 0;
}

double readSensorValue(unsigned char channel){
	int adcValue = -1;
	double value = -1;
	adcValue = read_mcp3208_adc(channel);
	switch(channel){
		case TEMP_CHANNEL:
			value = mcp_to_temperature(adcValue);	
			break;
		case HUMIDITY_CHANNEL:
			value = mcp_to_humidity(adcValue);
			break;
		case LIGHT_CHANNEL:
			value = mcp_to_light(adcValue);
			break;
		default:
			return -1;
			break;
	}
	return value;
}

int setSensorValue(unsigned char channel, double value){
	switch(channel){
		case TEMP_CHANNEL:
#ifdef DEBUG
			printf("set real temp : %lf\n",value);
#endif
			realTimeValue.temp[0] = value;	
			break;
		case HUMIDITY_CHANNEL:
			realTimeValue.humidity[0] = value;
			break;
		case LIGHT_CHANNEL:
			realTimeValue.light[0] = value;
			break;
		default:
			return -1;
			break;
	}
	return 0;
}
double getSensorValue(unsigned char channel){
	switch(channel){
		case TEMP_CHANNEL:
			return realTimeValue.temp[0];	
			break;
		case HUMIDITY_CHANNEL:
			return realTimeValue.humidity[0];
			break;
		case LIGHT_CHANNEL:
			return realTimeValue.light[0];
			break;
		default:
			return -1;
			break;
	}
}
	
void checkSensors(){
	printf("WaterTime : %d clock : %d\n",waterTime,clock());
	printf("water DIFF : %d\n",WATER_DIFF);
	if((clock() - waterTime) > WATER_DIFF){
		isWater = 0;
	}
	if(realTimeValue.temp[0] < settingValue.temp[0]){
		isCold = 1;
		#ifdef DEBUG
		printf("real %lf  setting  %lf\n",realTimeValue.temp[0],
				settingValue.temp[0]);
		#endif
	}
	else isCold = 0;
	if(realTimeValue.temp[0] > settingValue.temp[1])
		isHot = 1;
	else isHot = 0;
	if(realTimeValue.humidity[0] < settingValue.humidity[0])
		isDry = 1;
	else isDry = 0;
	if(realTimeValue.humidity[0] > settingValue.humidity[1]){
		if(isWater){
			isWet = 0;
		}else isWet = 1;
				
	}
	else isWet = 0;
	if(realTimeValue.light[0] < settingValue.light[1])
		isDark = 1;
	else isDark = 0;
	
}

double mcp_to_temperature(int adcAvg){
	double temp;
	double sampleMax = MCP3208_MAX;
	temp = (((adcAvg * VOLTAGE) / sampleMax) - 0.5) * 100;
	return temp;			 
}

double mcp_to_humidity(int adcAvg){
	double humi,humiMargin;
	double sampleMax = MCP3208_MAX;
	humiMargin = (sampleMax - ((float)adcAvg - HUMIDITY_MARGIN));
	if (humiMargin < 0)
	       	humiMargin = 0;
	humi = humiMargin / sampleMax * 100;
	return humi;
}

double mcp_to_light(int adcAvg){
	double light;
	double sampleMax = MCP3208_MAX;
	//가렸을 때 0~50. 기준값은 50정도로 보면 될듯? 
	//형광등 기준 책상에 있을 때 50-75정도 나옴. 
	light = (double)adcAvg ; 
	return light;
}


      
int read_mcp3208_adc(unsigned char adcChannel){
	unsigned char buff[3];
	int adcValue = 0;

	buff[0] = 0x06 | ((adcChannel & 0x07) >> 2);
	buff[1] = ((adcChannel & 0x07) << 6);
	buff[2] = 0x00;

		wiringPiSPIDataRW(SPI_CHANNEL, buff, 3);
		digitalWrite(CS_MCP3208, 0);
		buff[1] = 0x0f & buff[1];
		
		adcValue = (buff[1] << 8) | buff[2];
		
		digitalWrite(CS_MCP3208, 1);
	//printf("adc : %d\n",adcValue);	
	return adcValue;
}



