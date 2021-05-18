#include <stdint.h>
#include "Tachometer.h"

uint16_t Tachometer_FirstRightTime, Tachometer_SecondRightTime;
uint16_t Tachometer_FirstLeftTime, Tachometer_SecondLeftTime;
int Tachometer_RightSteps = 0;     // incremented with every step forward; decremented with every step backward
int Tachometer_LeftSteps = 0;      // incremented with every step forward; decremented with every step backward
enum TachDirection Tachometer_RightDir = STOPPED;
enum TachDirection Tachometer_LeftDir = STOPPED;

void Tachometer_Get(int32_t* leftSteps, int32_t* rightSteps) {
	*leftSteps = getEncoderLeftCnt();
	*rightSteps = getEncoderRightCnt();
}