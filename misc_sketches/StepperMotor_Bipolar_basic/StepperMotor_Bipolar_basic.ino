///This works for Bipolar Stepper motors interfaced with the L297 H-Bridge Motor Driver
//Refer to picture for pinout and schematic
//Bipolar uses 4 wires and has alot more torque

//Install the Arduino Stepper Library into the Energia IDEA, watch https://www.youtube.com/watch?v=0qwrnUeSpYQ (I think)
// Include the Arduino Stepper Library
#include <Stepper.h>

#define LP_LEFT_BTN  PUSH2
#define LP_RIGHT_BTN  PUSH1

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

void setupWaitBtn(uint8_t btn) {
    pinMode(btn, INPUT_PULLUP);
}

bool checkBtnPressed(uint8_t btnPin) {
	uint8_t btnCnt = 0;
	//Serial.println("Button Value is");
	//Serial.println(digitalRead(btnPin));
	while (digitalRead(btnPin) == 0) {
		delay(25);
		btnCnt++;
		//half a second check
		if (btnCnt >= 20) {
			return true;
		}
	}
	if (btnCnt < 20) {
		return false;
	}
}

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
  myStepper.step(direction*stepsPerRevolution/10);  //equates to value of 20 to move it a steady clip
  }

}
