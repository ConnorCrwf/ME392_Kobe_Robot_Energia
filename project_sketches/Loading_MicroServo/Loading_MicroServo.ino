/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>
// adjust this parameter to get proper position
#define TURN_TIME 15
#define TURN_TIME_COUNTER_CLK 6

Servo myservo;  // create servo object to control a servo
const int PWMpin = 17;
int clockWise_SpeedAccel = 98;    
int clockWise_SpeedDeccel = 0;

int counterClockWise_SpeedAccel = 98;    
int counterClockWise_SpeedDeccel = 180;

int goalSpeed = 97;

void setup() {
  myservo.attach(PWMpin);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
}

void loop() {

  //OPEN
  while(clockWise_SpeedAccel > clockWise_SpeedDeccel)
  {
    clockWise_SpeedAccel -= 1;
    myservo.write(clockWise_SpeedAccel);
    delay(TURN_TIME);
    Serial.println(clockWise_SpeedAccel);
    Serial.println("");
    Serial.println(goalSpeed);
  } 
  while(clockWise_SpeedDeccel < goalSpeed)
  {
    clockWise_SpeedDeccel += 1;
    myservo.write(clockWise_SpeedDeccel);
    delay(TURN_TIME);
    Serial.println(clockWise_SpeedDeccel);
    Serial.println("");
    Serial.println(goalSpeed);
  } 
  myservo.write(goalSpeed);
  Serial.println("here");
  delay(500);

  //CLOSE

  while(counterClockWise_SpeedAccel < counterClockWise_SpeedDeccel)
  {
    counterClockWise_SpeedAccel += 1;
    myservo.write(counterClockWise_SpeedAccel);
    delay(TURN_TIME_COUNTER_CLK);
  } 
  while(counterClockWise_SpeedDeccel > goalSpeed)
  {
    counterClockWise_SpeedDeccel -= 1;
    myservo.write(counterClockWise_SpeedDeccel);
    delay(TURN_TIME_COUNTER_CLK);
  } 
  
  }

  


  /*
  do {
//  Serial.println(reached);
  
    // in steps of 1 degree due to delay of 15 ms. this is their version of a dump encoder
//    pos += 1;
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
//    Serial.println("Pos is: ");
    Serial.println(pos);
    if (pos==goal) {
      reached = 1;
      delay(5000);
    }
//  }
  } while (reached == 0);
  /*
  if (pos == goal) {
    reached = 1;
  }
  }
  else
  {
    myservo.write(0);
    delay(15);
//    Serial.println("Reached");
  }

 */
//}

//  val = map(val, 0, 1023, 0, 180);     // scale it for use with the servo (value between 0 and 180); if wanted to use an analog signal like a pot to tell you where to go
