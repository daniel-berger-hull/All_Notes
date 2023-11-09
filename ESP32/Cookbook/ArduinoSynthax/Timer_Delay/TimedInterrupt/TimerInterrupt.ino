volatile int interruptCounter;
int totalInterruptCounter;
 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}
 
void setup() {
 
  Serial.begin(115200);
 
  // Step one is to defined a timer, and also the prescaller to use
  // The ESP32 timer clock runs at 80 MHz, but in the code below a prescaler
  // of 80 is set, then the timer 0 here will tick at 1 MHkz
  timer = timerBegin(0, 80, true);
  
  // Next step is to define which method to execute when to timer expirer
  // Here the function onTimer is executed
  timerAttachInterrupt(timer, &onTimer, true);

  // And finally, we have to set after how many ticks the timer has to fire
  // Here, it will count till it reaches 1 million microSec, so every seconds
  // The timer will fire
  timerAlarmWrite(timer, 1000000, true);

  // And once everything is set, just start the timer...
  timerAlarmEnable(timer);
 
}
 
void loop() {
 
  if (interruptCounter > 0) {
 
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
 
    totalInterruptCounter++;
 
    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(totalInterruptCounter);
 
  }
}
