#ifndef __SERVER_H__
#define __SERVER_H__

//모드

//명령관련
#define MODE_SUCCESS 		0	//명령 성공
#define MODE_FAIL 		1	//명령 실패

#define MODE_ALARM_TEMP 	10	//온도 알람
#define MODE_ALARM_HUMIDITY 	11	//습도 알람
#define MODE_ALARM_LIGHT	13	//빛 알람

#define MODE_SEND_DATA		20	
#define MODE_GET_DATA		100	//사용자가 데이터 요구
#define MODE_SET_DATA1		101	//사용자가 설정 데이터 설정

int init_server(void);	//return 값은 client
char* read_server(int);	//client를 인자로 받아 받은 String을 리턴
void write_server(int, char*);//client와 메시지를 인자로 받아 client로 보냄
void make_message(int, ...); //프로토콜 메시지 생성함수
char SNDmessage[30]; //보낼 메시지

int sock;


void disConnect(void);


#endif
