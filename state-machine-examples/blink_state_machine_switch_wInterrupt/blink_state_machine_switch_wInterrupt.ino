const byte interruptPin = 2;
const byte LEDpin = 5;
//  starts off as 0
  static uint8_t state = 0;
//  starts off as high
  static uint8_t switchState = 1;


void switchHandler()
{
  delay(200);
  Serial.print("Current value of State ");
  Serial.println(state);
  state = !state;
  Serial.print("Going to State ");
  Serial.println(state);
}

void blink(void)
{
  int prevLEDval = digitalRead(LEDpin);
  Serial.println("Value of LED is");
  Serial.println(prevLEDval);
  switch (state)
  {
    case 0:   // toggle the LED
//    could also use LED_BUILTIN onboard the arduino
      digitalWrite(LEDpin, !prevLEDval);
//      need this delay or else it won't blink
      delay(100);
      // handles events and state transition
      break;
    case 1:   // wait for the delay period
      digitalWrite(LEDpin, LOW);
      delay(1);
      break;
  }
}

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  pinMode(LEDpin, OUTPUT); 
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), switchHandler, RISING);
}

// the loop routine runs over and over again forever:
void loop() {
  blink();
}
