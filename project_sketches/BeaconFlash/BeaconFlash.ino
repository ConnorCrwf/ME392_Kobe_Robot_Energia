//all this code does is change the enable pin for a random pin so as to turn different beacons on or off
//one pin is high and the rest are low. 'val' pin has opposite value of the 'i' pins
//the interrupt function just changes the state and allows for another random pin to be selected (through use of flag)
//there is also a timer that writes a value of low to everything after 120 seconds

volatile int state = HIGH;
volatile int flag = HIGH;
int count = 0;
const int ledPin4=23;
const int ledPin5=24;
int ledPin;
int val;
int countIdle=0;
const int buzzer=14;

long int starttime;
long int endtime;
int loopCount=0;

void setup()
{
  Serial.begin(9600);
  
  pinMode(buzzer,OUTPUT);
  digitalWrite(buzzer,LOW);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  digitalWrite(11,HIGH);
  digitalWrite(12,HIGH);
  digitalWrite(13,HIGH);

  /* Enable internal pullup. 
   * Without the pin will float and the example will not work */
  pinMode(PUSH2, INPUT_PULLUP);
  attachInterrupt(PUSH2, blink, FALLING); // Interrupt is fired whenever button is pressed
  Serial.println(count);
}

void loop()
{
    if(flag) {
    count++;
    val=random(11,13);
    Serial.println(count);
    Serial.println(val);
    Serial.println(state);
    flag = LOW;
    //starttime = millis();
    //endtime = starttime;
    if( loopCount == 0)
{
starttime = millis();
}

  } 
  
loopCount++;
//if( (millis() - starttime) >= 1000*10)
//{
  for(int i=11;i<=13;i++)
  {
      if(state)
      {
        digitalWrite(i,!state);
        digitalWrite(val,state);
      }
      else
      {
        digitalWrite(i,state);
        digitalWrite(val,!state);
      }
      if( (millis() - starttime) > 1000*120 && flag==LOW){ //1000*120
        digitalWrite(i,LOW);
        digitalWrite(val,LOW);
        digitalWrite(buzzer, HIGH);
        delay(3000);
        digitalWrite(buzzer,LOW);
        while(true);
      }
  }
 
 digitalWrite(val,LOW);
}

void blink()
{
  Serial.println("BUTTON");
  state = !state;
  flag = HIGH;
}
