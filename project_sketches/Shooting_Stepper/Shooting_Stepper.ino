///This works for Bipolar Stepper motors interfaced with the L297 H-Bridge Motor Driver
//Refer to picture for pinout and schematic
//Bipolar uses 4 wires and has alot more torque

// Include the Arduino Stepper Library
#include <Stepper.h>
#include "SimpleRSLK.h"


String btnMsg = "Push left button on Launchpad to start demo.\n";


// Number of steps per output rotation
const int stepsPerRevolution = 200;

// Create Instance of Stepper library
//pinout right below was for Arduino Stepper Motor Tutorial. 
//Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);
//changed below to work for MSP432 available pins
//brown IN1  P2.3 - 34 , gray IN2 P2.4 - 38, purple IN3 P2.5 - 19, orange IN4 P3.5 - 32
//needs at least 40 mA and 7.5 volts for this to work
Stepper myStepper(stepsPerRevolution, 34, 38, 19, 32);
bool turn = false; 
int direction = 1;

void setup()
{
  Serial.begin(115200);
  //this make it so that value of button pin goes to zero when pressed
  setupWaitBtn(LP_LEFT_BTN);
  setupWaitBtn(LP_RIGHT_BTN);
  // set the speed at 60 rpm:
  myStepper.setSpeed(20);
  // initialize the serial port:
//  Serial.begin(9600);
  delay(2000);
}

void loop() 
{
  if(checkBtnPressed(LP_RIGHT_BTN))
  {
    direction = -1*direction;
  }
  if(checkBtnPressed(LP_LEFT_BTN))
  {
    Serial.println("Left Button Pressed");
    turn = !turn;
  }
  if (turn == true) {
  // step one revolution in one direction:
  //  Serial.println("Counter-clockwise");
  myStepper.step(direction*stepsPerRevolution/10);  //equates to value of 20 to move it a steady clip
//  delay(25);
  }
//  delay(25);
//  delay(1000);

//  // step one revolution in the other direction:
//  Serial.println("Clockwise");
//  myStepper.step(-stepsPerRevolution);
//  delay(100);
}
