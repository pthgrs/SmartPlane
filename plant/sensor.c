#include"sensor.h"

int pi; 
int spi; 

int initSensor(void){
   if(pi = pigpio_start(NULL , NULL)) <0) {
       return 1;
   }
   if(spi = spi_open(pi, 0, BAUD_RATE, 0)) <0){
      return 2;
   }
   return 0;
}

uint16_t getHumidity(int channel){
    uint16_t value;                                                             
    uint64_t sum = 0;                                                           
    uint16_t avg;                                                                                                                       
    char snd_buf[3];                                                            
    char rcv_buf[LOOP][3]; 
    
    avg = sum = 0; 
    snd_buf[0] = 0x18 | channel ; 
    
    for(int i=0 ; i< LOOP ; i++)
      spi_xfer(pi, spi, snd_buf, rcv_buf[i], 3); 
      
     for(int i=0; i<LOOP ; i++){
         value = ((rcv_buf[i][1] & 0x3f) <<8 ) | (rcv_buf[i][2] & 0xff) ;
         value = value >> 2; 
         sum += value;
     }

      avg = sum / LOOP; 
      
      return avg; 
}

void closeSensor(void){
   spi_close(pi, spi);
   pigpio_stop(pi);
}
      



