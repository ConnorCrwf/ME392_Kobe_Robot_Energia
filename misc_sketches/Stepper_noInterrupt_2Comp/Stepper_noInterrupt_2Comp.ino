// Include the Arduino Stepper Library
#include <Stepper.h>


// Number of steps per output rotation
const int stepsPerRevolution = 200;
// could use 16 if wanted to use a Pull_down resistor
const int ShootingCmdPin = 5;
int ShootingState = 0;
// Create Instance of Stepper library
//brown, gray, purple, orange
//Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);
Stepper myStepper(stepsPerRevolution, 34, 38, 19, 32);

void setup()
{
  
  pinMode(ShootingCmdPin, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(ShootingCmdPin), Windup, FALLING); 
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);
  // initialize the serial port:
  Serial.begin(9600);
  
}

void loop() 
{
  //This line is to just test if the motor works. Comment this then uncomment below to do at the program was intended
  myStepper.step(-stepsPerRevolution);

  /*
  ShootingState = digitalRead(ShootingCmdPin);
  if (ShootingState == HIGH) {     
Serial.println ("1");
  } 
  else {
Serial.println ("0");
myStepper.step(-stepsPerRevolution);
  }
  */
  
  //End Debug code
  delay(10);
}

void Windup() {
  Serial.println("Turning counter clockwise");
  myStepper.step(-stepsPerRevolution);
//  delay(10);
}
