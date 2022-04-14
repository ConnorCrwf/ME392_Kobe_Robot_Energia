#include "SimpleRSLK.h"
#include "Odometry.h"
#include "Line_Sensor.h"
#include "Infrared.h"
#include "States.h"

String btnMsg = "Push left button on Launchpad to start demo.\n";
unsigned long setupTimeOffset = 0;

void setup() {
  Serial.begin(9600);
  //Don't press until on the floor ready go or it could mess up the linesenor calibration
  setupWaitBtn(LP_LEFT_BTN);
  waitBtnPressed(LP_LEFT_BTN,btnMsg,RED_LED);
  setupRSLK();
  setupLed(RED_LED);
  //may want to move this into a specific library
  setupLineSensor();
  setupIRsensors();
  //nothing needed for bump initialization, in setupRSLK
  resetLeftEncoderCnt();
  resetRightEncoderCnt();
  Odometry_Init(0, 0, EAST); // facing North
  setMotorDirection(BOTH_MOTORS, MOTOR_DIR_FORWARD);
  enableMotor(BOTH_MOTORS);
  State_Init();
  delay(2000);
  setupTimeOffset = millis();
}

//long previousMillis = 0;        // will store last time LED was updated
//long previousMillis2 = 0;
//long interval = 30;           // interval at which to run Odom
//long interval2 = 4000;         //interval at which to stop motor

//extern int32_t MyTheta;
void loop() {
    waitCheck = false;
    static long previous_time = millis();
    waitCheck = Wait(previous_time, 1000);
    if (waitCheck) previous_time = millis();
  
//Probably need to return a struct out of UpdatePosition so that I can use the MyX MyY and MyTheta in other parts of the program
//This will also require not having any blocking code in the rest of the program which may be difficult to do since odometry will get off if you get stuck in a while loop that doen't call UpdatePosition for a period of time
  /*unsigned long currentMillis = millis();
  long timePast = (currentMillis - setupTimeOffset) - previousMillis;
  if(timePast > interval) {
    
      UpdatePosition(getEncoderLeftCnt(), getEncoderRightCnt());
      previousMillis = currentMillis - setupTimeOffset;
      Serial.println(MyTheta * .000383);
  }*/

  State_Action(waitCheck);
  //State_Delay();
  //maybe include update odometry here
  State_Event(waitCheck);
  State_Next(waitCheck);
}
