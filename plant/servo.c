#include<stdio.h>                                                               
#include<wiringPi.h>                                                            
#include<softPwm.h>                                                             

#define SERVO 1                                                                 

int main()                                                                      
{                                                                               
	char str;                                                                    
	if(wiringPiSetup() == -1) return 1;                                         

	softPwmCreate(SERVO, 0, 200);                                               

	while(1){                                                                   
		fputs(" select  c, r, l, d,  q : ",stdout);                               
		scanf("%c", &str);                                                      
		getchar();                                                              

		switch(str){                                                            
			case 'c' : softPwmWrite(SERVO,15);                               
					   break;                                                   
			case 'r' : softPwmWrite(SERVO,24);                                 
					   break;                                                  
						   //case 'l' : softPwmWrite(SERVO,5);                                 
						   //           break;                                                 
			case 'q' : return 0;                                                
					   break;                                                   
					   //case 'b' : softPwmWrite(SERVO, 12);                               
					   //           break; //as same "C"                                   
			case 'l' : softPwmWrite(SERVO, 8);                                  
					   break; //left 90                                         
					   //case 'd' : softPwmWrite(SERVO, 20);                               
					   //          break;                                                 
					   //case 'e' : softPwmWrite(SERVO, 25);                               
					   //          break; // same to 'r'                                  
			case 'd' : softPwmWrite(SERVO, 2);                                  
					   break; //180                                             
					   //case 'f' : softPwmWrite(SERVO, 1);                                
					   //          break; //same to 'g'.                                  
					   //case 't' : softPwmWrite(SERVO, 6);                                
					   //           break;                                                 
			default : softPwmWrite(SERVO,15);                                   
					  break;                                                    
		}                                                                       

	}                                                                           
	return 0;                                                                   
}     
