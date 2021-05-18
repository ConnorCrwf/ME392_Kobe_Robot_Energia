#ifndef TACHOMETER_H_
#define TACHOMETER_H_

//for setting Dir to a specific Constant that we can display to serial monitor
enum TachDirection {
	FORWARD, /**< Wheel is making robot move forward */
	STOPPED, /**< Wheel is stopped */
	REVERSE  /**< Wheel is making robot move backward */
};

//Function Declarations
void Tachometer_Get(int32_t* leftSteps, int32_t* rightSteps);


#endif /* TACHOMETER_H_ */