#include "SimpleRSLK.h"
#include "Odometry.h"

String btnMsg = "Push left button on Launchpad to start demo.\n";
unsigned long setupTimeOffset = 0;

void setup() {
  Serial.begin(115200);
  setupRSLK();
  /* Left button on Launchpad */
  setupWaitBtn(LP_LEFT_BTN);
  /* Red led in rgb led */
  setupLed(RED_LED);
  
  /* Wait until button is pressed to start robot */
  waitBtnPressed(LP_LEFT_BTN,btnMsg,RED_LED);
  delay(2000);
  /* Set the encoder pulses count back to zero */
  resetLeftEncoderCnt();
  resetRightEncoderCnt();
  Odometry_Init(0,0,EAST); // facing North
  /* Cause the robot to drive forward */
  setMotorDirection(BOTH_MOTORS,MOTOR_DIR_FORWARD);
  /* "Turn on" the motor */
  enableMotor(BOTH_MOTORS);

  setupTimeOffset = millis();
}


//using a timing variable to only call read tach and update odom after a specific period of time 
// so as not to not get in the way of other tasks
//look at BlinkWithoutDelay example for reference on how to incorporate periodic interrupts

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long previousMillis = 0;        // will store last time LED was updated
long previousMillis2 = 0;
long interval = 30;           // interval at which to run Odom
long interval2 = 4000;         //interval at which to stop motor

extern int32_t MyTheta;
void loop() {
  
  unsigned long currentMillis = millis();
  long timePast = (currentMillis - setupTimeOffset) - previousMillis;
  if(timePast > interval) {

      //TODO may need to conver thes into signed integers as this Encoder function returns unsigned
      //should only matter if going to go past 2^31 in number size which shouldn't happen
      //Don't need to return the x,y, and theta and pass into SoftLeftUntilTh since those global variables are updated with the Get_Odom function in UpdatePosition
      UpdatePosition(getEncoderLeftCnt(), getEncoderRightCnt());
      previousMillis = currentMillis - setupTimeOffset;
      Serial.println(MyTheta * .000383);
  }

//  SoftLeft();
  SoftLeftUntilTh(NORTH);
  waitBtnPressed(LP_LEFT_BTN,btnMsg,RED_LED);
  delay(2000);

//If want to stop motor after a certain amount of time
//includes offset for time taken to leave setup function (waiting for button press)
  /*
  long timePast2 = (currentMillis - setupTimeOffset) - previousMillis2;
  if(timePast2 > interval2) {
    Serial.println("Stopping Motor");
    Serial.println(timePast);
    disableMotor(BOTH_MOTORS);
    waitBtnPressed(LP_LEFT_BTN,btnMsg,RED_LED);
  }
  */

}
