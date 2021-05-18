///This works for Bipolar Stepper motors interfaced with the L297 H-Bridge Motor Driver
//Refer to picture for pinout and schematic
//Bipolar uses 4 wires and has alot more torque

// Include the Arduino Stepper Library
#include <Stepper.h>


// Number of steps per output rotation
const int stepsPerRevolution = 200;

// Create Instance of Stepper library
//pinout right below was for Arduino Stepper Motor Tutorial. 
//Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);
//changed below to work for MSP432 available pins
//brown IN1  P2.3 - 34 , gray IN2 P2.4 - 38, purple IN3 P2.5 - 19, orange IN4 P3.5 - 32
Stepper myStepper(stepsPerRevolution, 34, 38, 19, 32);


void setup()
{
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);
  // initialize the serial port:
//  Serial.begin(9600);
}

int count = 0;
void loop() 
{
  if (count == 0) {
  // step one revolution in one direction:
  Serial.println("Counter-clockwise");
  myStepper.step(stepsPerRevolution/4);
  count++;
//  delay(200);
  }

//  // step one revolution in the other direction:
//  Serial.println("Clockwise");
//  myStepper.step(-stepsPerRevolution);
//  delay(100);
}
