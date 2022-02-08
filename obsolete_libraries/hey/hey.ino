#include "States_debug.h"

String btnMsg = "Push left button on Launchpad to start demo.\n";
unsigned long setupTimeOffset = 0;

void setup() {
  Serial.begin(115200);
  State_Init();
}

//long previousMillis = 0;        // will store last time LED was updated
//long previousMillis2 = 0;
//long interval = 30;           // interval at which to run Odom
//long interval2 = 4000;         //interval at which to stop motor

//extern int32_t MyTheta;
void loop() {
  
//Probably need to return a struct out of UpdatePosition so that I can use the MyX MyY and MyTheta in other parts of the program
//This will also require not having any blocking code in the rest of the program which may be difficult to do since odometry will get off if you get stuck in a while loop that doen't call UpdatePosition for a period of time
  /*unsigned long currentMillis = millis();
  long timePast = (currentMillis - setupTimeOffset) - previousMillis;
  if(timePast > interval) {
    
      UpdatePosition(getEncoderLeftCnt(), getEncoderRightCnt());
      previousMillis = currentMillis - setupTimeOffset;
      Serial.println(MyTheta * .000383);
  }*/

  //State_Action();
  ////State_Delay();
  ////maybe include update odometry here
  //State_Event();
  //State_Next();
}
