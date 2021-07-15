#include <stdio.h>
#include "SimpleRSLK.h"
#include "Odometry.h"
#include "Line_Sensor.h"
#include "States_debug.h"
#include "StateEvents_debug.h"
#include "StateMachine.h"
#include "Shooting.h"
#include "Infrared.h"
#include "Energia.h"

static enum StateMachineStage rStage;
STyp* rState;
//may want to do something like the above to be able to use different types of rEvents. Use a pointer
Event rEvent;
ShootingEvent sEvent;
//LineType LineFound = NO_LINE;
int Origin_x;
int Origin_y;
int last_basket;
long shotTime;        // will store last time LED was updated
int activeBasketTheta;

/* ================================================== */
/* ----- High-Level State Steps (Init, Action, Delay, Event, Next) ----- */
/* ================================================== */

//TODO: Make an init function that sets rSTage, rEvent, and rStage
//this will be good and allow us to comment out one init and uncomment out another 
//if we want to change rStage, rState, rEvent for example I think we just pass them in as references just like was done in Odom_Get
void State_Init() {
    rStage = CALIB;
    rState = Init_Cal;
    ////assign it a State in Calib_1 stage initially.
    ////Initialize it to something otherwise it's just a null pointer after its declaration
    //rStage = SHOOTING;
    //rState = Init_Shooting;
    rEvent = NOTHING;
    
    //all other setup stuff is done in setup() function
    //TODO do a for loop like this in setupIRsensors();
}

void closingFnc() {

}




//Comments

//will need to store the current position in order to know how far to reverse
//probably best to just reset odom before to 0 0 EAST before commanding vehicle to reverse a set distance
//this won't really be East but it doesn't matter for these purposes. 
//TODO incorporate in another state that rotates 10 degrees from current position and then goes back to init
//or could just incorporate this into init from the start since we don't know where we are going to start

//Events don't have to be the same across the board, you cannot have more than the number of inputs defined in SType definition
//TODO May define a different type for each stage if this becomes an issue.
// i may need to add one more input for IR sensor or i could just store it as global variable during my State Event step
// Basically the more you take care of events here, the less logic you'll have to do in the Action functions. For example, 
//I could have just used one position for LINE_DETECT, then just called a rotate function. Within that rotate function, 
//I would look at global moving average for Line Sensor values. This would definitely simplify my state machine

//this passing of odom values directly into the state machine may not be the best as it forces you to set your values here which you may not be able to change
//might be best to change a global odom goal variable in the State_Event function for example

//Calibration State notes
//Calibration State, Start with this first before getting into handling the inputs
// the numbers in front of state indicate what part of the stage the robot is in
//1 indicates it's still in the find and reason about first line detected
//2 indicates it's finding the second line, can probably reason about which line you have centered on if you are getting a non-1024 reading on IR sensor
// this 2 state may be as simple as just reading the line sensor and doing a moving average over a certain amount of time
//3 indicates it has found the middle line and
//4 indicates it has determine which side of the board it is on
// there will be some states where we stay in a while loop and then exit the whie loop once a certain condition is met
// this will block the executation of the state machine and will have to be handled appropriately
// by setting a new state in that while loop
// will need to store x and y positions throughout the program and then reset odom at some point

//Notes on Alex's original State_Next
 //This -> dereferences the Robot State struct pointer, which is what rState is
//Are enums basically the same thing as integers indices since the StateMachine has different states based on which column is called
//this seems like a strange way to do it. Couldn't we just say
//CalibraitonStateMachine = CalibrationStateMachine->next_state_by_event[rNaveEvent];
//rState = &CalibrationStateMachine[rState->next_state_by_event[rCalEvent]];
//Could just set a rCalEvent in a specific output function since we should only experience certain events based on what state and output function we are executing
//Could also use periodic or event-driven interrupts to change the rCalEvent global variable
//Why is this static? It's because we want it to maintain some kind of value across files?
//static const RobotState* rState;

//TODO try this out by just doing a default delay state right after Action
//maybe don't even use this since all it does it potentially block our code
//could even turn this into a get Objective function that based on rStage will either look at odom or ir sensor or both
//void State_Delay() {
//    delay(rState->delayTime);   // figure out a non-blocking way to do this, can maybe use Alex's timer thing. that way we could still handle bump interrupts
//}
