
void blink(void)
{
  const uint32_t STATE_0_DURATION = 5000;
  const uint32_t STATE_1_DURATION = 2000;
  //  static allows the value to be kept after 
// exiting the function and going back into loop
//it doesn't get set to zero when the function enters back in
//that just happens once
  static uint8_t state = 0;
Serial.print("State is ");
Serial.println(state);
  static uint32_t timeLastTransition = 0;
  uint32_t curTime;
  uint32_t timePassed;

  curTime = millis();
  timePassed = curTime - timeLastTransition;
//  Serial.print("Current State is ");
//  Serial.println(state);
//  Serial.println(timePassed);
 
  switch (state)
  {
    case 0:   // toggle the LED
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      if (timePassed >= STATE_0_DURATION) {
        state = 1;
        Serial.println("Going to State 1");
        delay(2000);
        timeLastTransition = millis();
      }
      break;
    case 1:   // wait for the delay period
      if (timePassed >= STATE_1_DURATION) {
        state = 0;
        Serial.println("Going to State 0");
        delay(2000);
        timeLastTransition = millis();
      }
      break;
  }
}


// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); 
}

// the loop routine runs over and over again forever:
void loop() {
  blink();
  
}
