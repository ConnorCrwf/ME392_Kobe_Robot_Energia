#include "StateEvents.h"
#include "States.h"
#include "StateMachine.h"
#include "SimpleRSLK.h"
#include "Odometry.h"
#include "Line_Sensor.h"
#include "Infrared.h"

//Queue<int> lineQueue;
#define LINE_DEQUE_SIZE 10
Deque<int> lineData = Deque<int>(LINE_DEQUE_SIZE);

//if this doesn't work go back to using just a regular Event event
void checkBump(Event& event) {
    //read bump
    if (readBumpSwitches() == true) {
        event = BUMP;
    }
}

//No real reason why I'm passing in one by reference and one by pointer, idk
void checkOdomGoal(STyp* state, Event& event) {
    int32_t Error; // in 0.0001cm or in 2*pi/16384 radians
    int32_t tolerance;
    int curX; int curY; int curTheta;
    //read odometry always
    UpdatePosition(getEncoderLeftCnt(), getEncoderRightCnt());
    Serial.println();
    curX = Odometry_GetX();
    curY = Odometry_GetY();
    curTheta = Odometry_GetAngle();
    Serial.print("curX in our scale is : ");
    Serial.println(curX * 0.0001);
    Serial.print("curY in our scale is : ");
    Serial.println(curY * 0.0001);
    Serial.print("curTheta in our scale is : ");
    Serial.println(curTheta * 0.000383);
    Serial.println();
    Serial.print("Goal Direction is: ");
    Serial.println(state->curGoal.direction);
    if (state->curGoal.direction == "None") {}
    // if statement to determine if goal has been reached; based on element of nested struct in state machine
    else if (state->curGoal.direction != "None") {
        if (state->curGoal.direction == "X") {
            //we can compare after we get x, y with Odom_Xand Odom_Y after running Update Position
            int goalX = state->curGoal.goaVals[0];
            Serial.print("goalX in our scale is : ");
            Serial.println(goalX * 0.0001);
            Error = goalX - curX;   // in 0.0001cm
            tolerance = XYTOLERANCE;
        }
        else if (state->curGoal.direction == "Y") {
            int goalY = state->curGoal.goaVals[1];
            Serial.print("goalY in our scale is : ");
            Serial.println(goalY * 0.0001);
            Error = goalY - curY;   // in counts of 0.0001cm
            tolerance = XYTOLERANCE;
        }
        else if (state->curGoal.direction == "Theta") {
            int goalTheta = state->curGoal.goaVals[2];
            Serial.print("goalTheta in our scale is : ");
            Serial.println(goalTheta * 0.000383);
            Error = goalTheta - curTheta;   // in 0.000383 rads
            if (Error >= PI_CUSTOM)Error = Error - TWOPI_CUSTOM;  // -8192 to +8191
            if (Error < -PI_CUSTOM)Error = Error + TWOPI_CUSTOM;  // -8192 to +8191
            tolerance = THETATOLERANCE;
            Serial.print("Actual Tolerance is : ");
            Serial.println(tolerance);
        }
        int32_t goal = abs(Error);      // in 0.0001 in or 2*pi/16384 radians depending on if looking for x/y or theta
        Serial.print("Error in our scale is : ");
        if (state->curGoal.direction != "Theta") Serial.println(goal * 0.0001);
        else Serial.println(goal * 0.000383);
        Serial.print("Actual Error is : ");
        if (state->curGoal.direction != "Theta") Serial.println(goal);
        else Serial.println(goal);
        if (goal < tolerance) event = GOAL_REACHED;
    }
    // another way is to turn on a global variable flag but that will require alot of custom functions for each state and is not very modular

}

void checkCenterIRGoal(int* angle, Event& event) {
    //maybe turn this into a moving average check instead of a one-off
    if (checkCenterIR()) {
        *angle = Odometry_GetAngle();
        event = GOAL_REACHED;
    }

}

void checkLine(Event& event) {
    
    updateLineDataHistory();
    int nonZero_count = getNonZeroCount();
    //one stray value of 2000 amonst alot of zeros would put average at 200 immediately for last 10 values
    //DONE: can probably avoid setting GOAL_REACHED in lineup function now with this check
    if (nonZero_count > 5) event = LINE_DETECT;
    else return;
}

void updateLineDataHistory()
{
    //read line sensor, still need to do setup stuff
    uint32_t linePos = readLineSensor();
    /*Serial.print("Line Pos is : ");
    Serial.println(linePos);*/
    //even though it's initialized to a certain size, count is only concerned with what has been added to it after intilization
    int sizeLineData = lineData.count();
    //initialize only once
    if (sizeLineData < 2) {
        for (int i = 0; i < LINE_DEQUE_SIZE + 1; i++) {
            lineData.push_back(0);
        }
    }
    /*Serial.print("Size of LineData is : ");
    Serial.println(sizeLineData);*/
    lineData.pop_front();
    lineData.push_back(linePos);
}

int getNonZeroCount() {
    //use a filter that counts how many times zero has been returned
    //if there has been three of them, then return zero for linePos becaus eyou know it's not a fluke
    // can probably do this filter in the LineSensor library (that I need to make) 
    int* data = lineData.data_array();
    int nonZero_count = 0;
    for (int i = 0; i < LINE_DEQUE_SIZE + 1; i++) {
        //why don't i have to dereference data before I use it?
        if (data[i] > 50) nonZero_count++;
        /*Serial.print("data [");
        Serial.print(i);
        Serial.print("] : ");
        Serial.println(data[i]);*/
    }
    return nonZero_count;
}

void checkShot(ShootingEvent& event, int& last_basket, long& shotTime) {
    int active_basket = Get_Basket();
    Serial.print("Basket Active is : ");
    Serial.println(active_basket);
    Serial.print("Last Basket is : ");
    Serial.println(last_basket);
    long interval = 3000;           // interval at which to run Odom
    unsigned long currentMillis = millis();
    long timePast = currentMillis - shotTime;
    if (timePast > interval) {
        if (active_basket != last_basket) event = SHOT_MADE;
        else event = SHOT_MISSED;
    }
}