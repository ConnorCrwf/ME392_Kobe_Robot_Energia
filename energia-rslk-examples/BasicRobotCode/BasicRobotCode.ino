/*
 * Energia Robot Library for Texas Instruments' Robot System Learning Kit (RSLK)
 * Derived from Line Following Example
 *
 * Summary:
 * This example has the TI Robotic System Learning Kit (TI RSLK) follow a line
 * using a basic line following algorithm. This example works on a dark floor with
 * a white line or a light floor with a dark line. The robot first needs to be calibrated
 * Then place the robot on the hit the left button again to begin the line following.
 *
 * There is also code to read the encoder from a DC motor. 
 *
 * How to run:
 * 1) Push left button on Launchpad to have the robot perform calibration.
 * 2) Robot will drive forwards and backwards by a predefined distance.
 * 3) Place the robot center on the line you want it to follow.
 * 4) Push left button again to have the robot begin to follow the line.
 * 5) Push right button to read encoder on the other DC motor
 *
 * Parts Info:
 * o Black eletrical tape or white electrical tape. Masking tape does not work well
 *   with IR sensors.
 *
 * Learn more about the classes, variables and functions used in this library by going to:
 * https://fcooper.github.io/Robot-Library/
 *
 * Learn more about the TI RSLK by going to http://www.ti.com/rslk
 *
 * created by Franklin Cooper Jr.
 * edited by Ann Majewicz Fey
 *
 * This example code is in the public domain.
 */

#include "SimpleRSLK.h"

// LINE SENSOR DEFINITIONS
uint16_t sensorVal[LS_NUM_SENSORS];
uint16_t sensorCalVal[LS_NUM_SENSORS];
uint16_t sensorMaxVal[LS_NUM_SENSORS];
uint16_t sensorMinVal[LS_NUM_SENSORS];

// DC MOTOR CONTROL AND ENCODER PINS
const int PWMoutp = 9; //P6_5
const int PWMoutn = 10; //P6_4
const int PWMspeed = 38; //P2_4

const int encoder0PinA = 60; //P3_5
const int encoder0PinB = 32; //P8_3
volatile unsigned int encoder0Pos = 0;
double motorSpeed = 0.0;
unsigned int encoderPosLast = 0;
int state = 1; 

void setup()
{
	Serial.begin(115200);

	setupRSLK();
	/* Left button on Launchpad */
	setupWaitBtn(LP_LEFT_BTN);
	/* Red led in rgb led */
	setupLed(RED_LED);
	clearMinMax(sensorMinVal,sensorMaxVal);
 
 // DC MOTOR SETUP
 pinMode(encoder0PinA, INPUT_PULLUP);
 pinMode(encoder0PinB, INPUT_PULLUP);
 pinMode(PWMoutp, OUTPUT);
 pinMode(PWMoutn, OUTPUT);
 pinMode(PWMspeed, OUTPUT);
 attachInterrupt(encoder0PinB, doEncoder, CHANGE); // Interrupt is fired whenever button is pressed
  Serial.println("start");
  state = 0; 
}

void floorCalibration() {
	/* Place Robot On Floor (no line) */
	delay(2000);
	String btnMsg = "Push left button on Launchpad to begin calibration.\n";
	btnMsg += "Make sure the robot is on the floor away from the line.\n";
	/* Wait until button is pressed to start robot */
	waitBtnPressed(LP_LEFT_BTN,btnMsg,RED_LED);

	delay(1000);

	Serial.println("Running calibration on floor");
	simpleCalibrate();
	Serial.println("Reading floor values complete");

	btnMsg = "Push left button on Launchpad to begin line following.\n";
	btnMsg += "Make sure the robot is on the line.\n";
	/* Wait until button is pressed to start robot */
	waitBtnPressed(LP_LEFT_BTN,btnMsg,RED_LED);
	delay(1000);
  state = 1;

	enableMotor(BOTH_MOTORS);
}

void simpleCalibrate() {
	/* Set both motors direction forward */
	setMotorDirection(BOTH_MOTORS,MOTOR_DIR_FORWARD);
	/* Enable both motors */
	enableMotor(BOTH_MOTORS);
	/* Set both motors speed 20 */
	setMotorSpeed(BOTH_MOTORS,20);

	for(int x = 0;x<100;x++){
		readLineSensor(sensorVal);
		setSensorMinMax(sensorVal,sensorMinVal,sensorMaxVal);
	}

	/* Disable both motors */
	disableMotor(BOTH_MOTORS);
  
}

bool isCalibrationComplete = false;
void loop()
{
  bool hitObstacle = false;
	uint16_t normalSpeed = 10;
	uint16_t fastSpeed = 20;

	/* Valid values are either:
	 *  DARK_LINE  if your floor is lighter than your line
	 *  LIGHT_LINE if your floor is darker than your line
	 */
	uint8_t lineColor = DARK_LINE;

	/* Run this setup only once */
	if(isCalibrationComplete == false) {
		floorCalibration();
		isCalibrationComplete = true;
	}

    /* Keep checking if the robot has hit an object */
  while(!hitObstacle) {
    /* Loop through all bump switchees to see if it has been pressed */
    for(int x = 0;x<TOTAL_BP_SW;x++)
    {
      /* Check if bump switch was pressed */
      if(isBumpSwitchPressed(x) == true) {
        hitObstacle = true;
        state = 2;
        break;
      }
    }
  }

 // Simple State Machine (line follow or spin)
 switch(state){
  case 1: // Line follow
  {
    analogWrite(PWMspeed, 0); // starts around 145
    readLineSensor(sensorVal);
    readCalLineSensor(sensorVal,
            sensorCalVal,
            sensorMinVal,
            sensorMaxVal,
            lineColor);

      uint32_t linePos = getLinePosition(sensorCalVal,lineColor);

     if(linePos > 0 && linePos < 3000) {
      setMotorSpeed(LEFT_MOTOR,normalSpeed);
      setMotorSpeed(RIGHT_MOTOR,fastSpeed);
      } else if(linePos > 3500) {
        setMotorSpeed(LEFT_MOTOR,fastSpeed);
        setMotorSpeed(RIGHT_MOTOR,normalSpeed);
      } else {
        setMotorSpeed(LEFT_MOTOR,normalSpeed);
        setMotorSpeed(RIGHT_MOTOR,normalSpeed);
      }
  }
  break; 
  case 2: // Spin DC motor
  {
    digitalWrite(PWMoutp, HIGH);
    digitalWrite(PWMoutn, LOW);
    analogWrite(PWMspeed, 145); // starts around 145
    //SOME EQUATION HERE TO CONVERT COUNTS TO POSITION OR SPEED
   //motorSpeed = (encoder0Pos-encoderPosLast)/0.01;
     Serial.println(encoder0Pos); // motorSpeed
    encoderPosLast = encoder0Pos;

    delay(100);

    hitObstacle = false;
    state = 1;
  }
  break;
 
  default:
  {
    /* Disable both motors */
  disableMotor(BOTH_MOTORS);
  analogWrite(PWMspeed, 0);
  }
  break;
  
 }

	
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
