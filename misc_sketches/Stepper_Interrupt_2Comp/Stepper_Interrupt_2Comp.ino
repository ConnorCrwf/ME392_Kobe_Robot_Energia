// Include the Arduino Stepper Library
#include <Stepper.h>


// Number of steps per output rotation
const int stepsPerRevolution = 200;
// could use 16 if wanted to use a Pull_down resistor
const int ShootingCmdPin = 2;
int ShootingState = 0;
// Create Instance of Stepper library
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup()
{
  
  pinMode(ShootingCmdPin, INPUT_PULLUP);  // changed from INPUT_PULLUP
  attachInterrupt(digitalPinToInterrupt(ShootingCmdPin), Windup, FALLING); 
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);
  // initialize the serial port:
  Serial.begin(9600);
  
}

void loop() 
{
  //Debug Code
  
  ShootingState = digitalRead(ShootingCmdPin);
  if (ShootingState == HIGH) {     
Serial.println ("1");
  } 
  else {
Serial.println ("0");
//myStepper.step(-stepsPerRevolution);
  }
  
  //End Debug code
  delay(10);
}

void Windup() {
  Serial.println("Turning counter clockwise");
//  delay(100);
  myStepper.step(stepsPerRevolution);

}
