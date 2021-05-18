const int PWMoutp = 9;
const int PWMoutn = 10;
const int PWMspeed = 31;

const int encoder0PinA = 3;
const int encoder0PinB = 4;
volatile unsigned int encoder0Pos = 0;
double motorSpeed = 0.0;
unsigned int encoderPosLast = 0;




void setup()
{
  // put your setup code here, to run once:
    Serial.begin(9600);
 pinMode(encoder0PinA, INPUT_PULLUP);
 pinMode(encoder0PinB, INPUT_PULLUP);
 pinMode(PWMoutp, OUTPUT);
 pinMode(PWMoutn, OUTPUT);
 pinMode(PWMspeed, OUTPUT);
 // encoder0PinA starts high because of how it is configured above
 // hardware causes the interrupt to change, nothing in software
 // when the motor turns it toggles the encoder0PinA somehow
 // most likely when the magnet or led-disc mechanism indicates an incremental rotation
 attachInterrupt(encoder0PinA, doEncoder, CHANGE); 
  
}

void loop()
{
  // put your main code here, to run repeatedly:
  
   //read the pushbutton value into a variable
 // int EncARead = digitalRead(EncA);
 // int EncBRead = digitalRead(EncB);
  
  // turn on motor A
  digitalWrite(PWMoutp, HIGH);
  digitalWrite(PWMoutn, LOW);
  
  
  analogWrite(PWMspeed, 90);
  // analogWrite(PWMoutn,0);
motorSpeed = (encoder0Pos-encoderPosLast)/0.01;

  //print out the value of the pushbutton
  Serial.println(motorSpeed);
    encoderPosLast = encoder0Pos;

  delay(10);

  
}

void doEncoder(){
  if (digitalRead(encoder0PinA) == HIGH) {   // found a low-to-high on channel A
    if (digitalRead(encoder0PinB) == LOW) {  // check channel B to see which way
                                             // encoder is turning
      encoder0Pos = encoder0Pos - 1;         // CCW
    } 
    else {
      encoder0Pos = encoder0Pos + 1;         // CW
    }
  }
  else                                        // found a high-to-low on channel A
  { 
    if (digitalRead(encoder0PinB) == LOW) {   // check channel B to see which way
                                              // encoder is turning  
      encoder0Pos = encoder0Pos + 1;          // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }
      
  }
  if (encoder0Pos >= 48){encoder0Pos = 0;}
  //Serial.println (encoder0Pos, DEC);          // debug - remember to comment out
                                              // before final program run
  // you don't want serial slowing down your program if not needed
}
