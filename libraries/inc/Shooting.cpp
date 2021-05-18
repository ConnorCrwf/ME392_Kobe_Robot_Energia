#include "Shooting.h"
#include <Stepper.h>
#include "Energia.h"

// Number of steps per output rotation
const int stepsPerRevolution = 200;

//brown  P2.3 - 34 , gray P2.4 - 38, purple P2.5 - 19, orange P3.5 - 32
Stepper myStepper(stepsPerRevolution, 34, 38, 19, 32);

int count = 0;
void Shoot()
{
	if (count == 0) {
		myStepper.setSpeed(30);
		count++;
	}

	if (count == 1) {
		// step one revolution in one direction:
		Serial.println("Shooting Counter-clockwise");
		myStepper.step(stepsPerRevolution);
		count++;
	}


}

void dummy() {
}