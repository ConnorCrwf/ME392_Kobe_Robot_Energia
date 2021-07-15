#include "StateEvents_debug.h"
#include "States_debug.h"
#include "StateMachine.h"
#include "SimpleRSLK.h"
//#include "Odometry.h"
//#include "Line_Sensor.h"
//#include "Infrared.h"
//#include "Deque.h"

//Queue<int> lineQueue;
#define LINE_DEQUE_SIZE 10
//Deque<int> lineData = Deque<int>(LINE_DEQUE_SIZE);

//if this doesn't work go back to using just a regular Event event
void checkBump(Event& event) {
    //read bump
    if (readBumpSwitches() == true) {
        event = BUMP;
    }
}








