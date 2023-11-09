
#include "freertos/FreeRTOS.h"


#define NOP() asm volatile ("nop")

int i = 0;


int rep = 0;


unsigned long IRAM_ATTR microWait()
{
    return (unsigned long) (esp_timer_get_time());
}
void IRAM_ATTR delayMicro(uint32_t us)
{
    uint32_t m = micros();
    if(us){
        uint32_t e = (m + us);
        if(m > e){ //overflow
            while(micros() > e){
                NOP();
            }
        }
        while(micros() < e){
            NOP();
        }
    }
}

void setup() 
{            
  Serial.begin(115200);
}


// void loop runs over and over again
void loop() 
{
  delayMicro(50);
}
