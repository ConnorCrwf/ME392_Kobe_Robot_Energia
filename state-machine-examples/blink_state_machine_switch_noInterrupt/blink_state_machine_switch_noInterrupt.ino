const byte switchPin = 2;
//  starts off as 0
  static uint8_t state = 0;
//  starts off as high
  static uint8_t switchState = 1;

void switchHandler()
{

//  if switch is pressed, its value goes to 0. must hold down switch to stay in state 1
  int switchStateCur = digitalRead(switchPin);
  Serial.print("Current value of Switch ");
  Serial.println(switchStateCur);
  Serial.print("Previous value of Switch ");
  Serial.println(switchState);
  if (switchStateCur != switchState) {
        state = !state;
        Serial.print("Going to State ");
        Serial.println(state);
        delay(2000);
      }
  switchState = switchStateCur;
  Serial.print("New Stored value of Switch ");
  Serial.println(switchState);
  delay(1);        // delay in between reads for stability
}

void blink(void)
{
  switch (state)
  {
    case 0:   // toggle the LED
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      // handles events and state transition
      switchHandler();
      break;
    case 1:   // wait for the delay period
      switchHandler();
      break;
  }
}

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(switchPin, INPUT_PULLUP);
}

// the loop routine runs over and over again forever:
void loop() {
  blink();
}
