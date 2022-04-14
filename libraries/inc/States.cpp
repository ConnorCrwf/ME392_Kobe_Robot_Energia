#include <stdio.h>
#include "SimpleRSLK.h"
#include "Odometry.h"
#include "Line_Sensor.h"
#include "States.h"
#include "StateEvents.h"
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
bool waitCheck;
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

//TODO Do a Check here to see what Stage you are in because the pointer might not be rState
//TODO maybe do a different prefix for state for each stage. that would simplify the confusion
void State_Action(bool waitCheck) {
    if(waitCheck) Serial.println(rState->stateName);
    //char Name[15];
    //strcpy(Name, rState->stateName);
    //Serial.print("State is");
    //Serial.println(Name);
    rState->CmdPt();
}

//return different rCalEvents based on those things
//this function has to cover so many cases we break up what it can return by what stage it is in
//TODO ask Dr. Fey how we check how often this is called versus how long it takes
//i think we put times at the beginning and end and then create two Buffer arrays with a size of maybe 50
//one array is time beginning time when each was called and then second array is duration where you get diff between end tme and beginning time

//With state machiens you do one of the following:
//1. the logic to figure out what to do with different inputs here
//2. Custom ponters that change the rEvent or rState. Messy because this could be overwritten by State_Event
//3. If don't want to use a specific input, then just revert state back to what is already is in the State Transition Table.
// basically just ignore that event if we want by setting us back to the same state we are already in
//problem with this is that one rEvent may overwrie another one, depending on where in the state transition table it is.
//fix to this is to return from State_Event if rEvent != NOTHING after each new input or within the input goal function
//another problem with this is that your number of inputs can get too big
//fix to this is to use different inputs for each Stage. 
//could always use the same enum Events and if they're not used in a particular stage, then just stick with current state
//could write UNUSED next to each case Below
//TODO maybe do a different prefix for each event type for each stage. that would simplify the confusion. just make sure that we still use the same number of inputs so that we can keep on using SType
//otherwise we'll have to do checks to in State_Next and State_Action
void State_Event(bool waitCheck) {
    /*Serial.print("waitCheck is: ");
    Serial.println(waitCheck);*/
    //if (waitCheck) previous_time = millis();
    //based on which stage you are in, will want to return a specific set of rCalEvents
    //return a boolean from each of these to be True if rEvent was changed to someting different than it was before,
    //then can just say if (checkBump(rEvent) break, then you won't have to deal with overwriting events
    if (rEvent == GOAL_REACHED) return;
    else {
        switch (rStage)
        {
            //return either NOTHING, BUMP, LINE_DETECT, or GOAL_REACHED
            //TURN all this into a functio nthat goes into Calib1 library
        case CALIB:
            //initialize to nothing. it will be overwritten if any of the other triggers happen
            rEvent = NOTHING;
            //checkBump(rEvent);
            //checkOdomGoal(rState, rEvent,waitCheck);
            checkLine(rEvent, waitCheck);
            if (rState == Init_Cal)
            {
                //checkCenterIRGoal(&activeBasketTheta, rEvent);
                //rEvent = GOAL_REACHED;
                //checkLine(rEvent,waitCheck);
            }
            //else checkLine(rEvent,waitCheck);
            //will be an enum 0,1,2, or 3
            if (waitCheck) {
                Serial.print("State Event is: ");
                Serial.println(rEvent);
            }
            break;
        case NAVIGATION:
            //initialize to nothing. it will be overwritten if any of the other triggers happen
            rEvent = NOTHING;
            checkOdomGoal(rState, rEvent, waitCheck);
            Serial.print("State Event First Check is: ");
            Serial.println(rEvent);
            checkBump(rEvent);
            //checkLineGoal();
            //if ((rState != Init1_Nav) || (rState != Init2_Nav) || (rState != Init3_Nav)) checkLineGoal();
            //will be an enum 0,1,2, or 3
            Serial.print("State Event Second Check is: ");
            Serial.println(rEvent);
            break;
        case SHOOTING:
            sEvent = SHOT_NOTHING;
            checkShot(sEvent, last_basket, shotTime);
            Serial.print("State Event is: ");
            Serial.println(sEvent);
            break;
        /* Not sure if we need these or not
        case CALIB_2:
            //initialize to nothing. it will be overwritten if any of the other triggers happen
            rEvent = NOTHING;
            checkBump(rEvent);
            checkOdomGoal(rState, rEvent);

            //checkLineGoal();
            if (rState != Rvrs2_Cal2) checkLineGoal(rEvent);
            //will be an enum 0,1,2, or 3
            //Serial.print("State Event is: ");
            //Serial.println(rEvent);
            break;
        */

        }
    }
}

//within advance(which occurs in the last state), we change the stage.should probably also change rState to a state in the next stage StateMachine
//because if we don't then we'll be in the wronge rStage case referring to an event input that would only occur in next stage
void State_Next(bool waitCheck) {
    const String lastStateName = rState->stateName;
    //maybe don't need a pointer here?
    STyp* prevState = rState;
    if (rStage == SHOOTING) rState = rState->Next[sEvent];
    else rState = rState->Next[rEvent];
    if (rState->stateName != lastStateName)
    {
        //kinda like a destructor
        prevState->ClosingPt();
        Serial.println();
        Serial.println("New State is: "); Serial.println(rState->stateName);
        Serial.println();
    }
}

/* ================================================== */
/* ----- These are functions avaiable at all stages and states, takes into account which Stage we are in ----- */
/* ================================================== */

// Advance to the next stage of the plan
void advance() {
    int curX;
    int curY;
    //Serial.print("rStage enum is : ");
    //Serial.println(rStage);
    switch (rStage) {
    case CALIB:
        rStage = NAVIGATION;
        rState = Init1_Nav;
        break;
    case NAVIGATION:
        rStage = SHOOTING;
        rState = Init_Shooting;
        curX = Odometry_GetX();
        Serial.print("curX in our scale is : ");
        Serial.println(curX * 0.0001);
        curY = Odometry_GetY();
        Serial.print("curY in our scale is : ");
        Serial.println(curY * 0.0001);
        break;
    case SHOOTING:
        int active_basket = Get_Basket();
        Serial.print("Basket Active is : ");
        Serial.println(active_basket);
        /*rStage = NAVIGATION;
        rState = Init1_Nav;*/
        break;
        /*
     case CALIB_2:
         //first thing when moving to Navigation will be navigate to center of board and do a verification on our line sensors
         //could even maybe call its Calib_3 and have Navigation specifically devoted to dealing with the IR_sensor

         rStage = NAVIGATION;
         //first state will be going to center point
         rState = Init1_Nav;
         curX = Odometry_GetX();
         Serial.print("curX in our scale is : ");
         Serial.println(curX * 0.0001);
         curY = Odometry_GetY();
         Serial.print("curY in our scale is : ");
         Serial.println(curY * 0.0001);
         Serial.print("Origin X in our scale is : ");
         Serial.println(Origin_x * 0.0001);
         Serial.print("Origin Y in our scale is : ");
         Serial.println(Origin_y * 0.0001);
         break;
     */
    }
}

//may want to change the rEvent to something else here if Odom or bump or line tells us something, but by default handle it in State_Event()
//need to make sure you don't overwrite rEvent set here in the the State_Event thing
bool lineup_flag = false;
void lineup() {
    //TODO instead of reading it again just use Get_LinePos() to access that private variable
    uint32_t linePos = readLineSensor_Kobe();
    //Serial.print("lineDataAverage is ");
    //Serial.println(LineDataAverage);
    static uint32_t count = 0;
    if (lineup_flag) {
        if ((linePos >= 3100) && (linePos <= 3400)) return;
        else if ((linePos > 55) && (linePos < 3100)) SoftRight();
        else if ((linePos > 3400) && (linePos < 7800)) SoftLeft();
        //else means that now we're not reading any black lines and we should be lined up perpendicular to line direction
        else { disableMotor(BOTH_MOTORS); Set_Event(GOAL_REACHED); }
     }
    else {
        if ((linePos >= 3100) && (linePos <= 3400)) {
            Serial.println("Hit Line, Keep going");
            lineup_flag = true;
            //do it this way instead of
            //Set_State(COMPLETE_CAL1);
            //because that way wouldn't go to next state when State_Next comes around due to NOTHING being the default input
            //it will think it's in the same state and the motors won't stop

            //Set_Event(GOAL_REACHED);
            //disableMotor(BOTH_MOTORS);
            return;
        }
        else if ((linePos > 55) && (linePos < 3100)) {
            SoftLeft();
            //rEvent remains the same
        }
        else if ((linePos > 3400) && (linePos < 7800)) {
            SoftRight();
            //rEvent remains the same
        }
    }
}

//maybe send to another state before Complete to let it move little bit up to line.
//this intermediate state nees to do more than run a counter that State_event would check against a parameter to see when to move to COMPLETE_CAL1
//it needs to go until no line is detected

// Default closing function
void closingFnc() {
    //always do this
    //turns motors off when going to another state, this may not be exactly what we want, but it's ok for now
    disableMotor(BOTH_MOTORS);
    enableMotor(BOTH_MOTORS);
    setMotorDirection(BOTH_MOTORS, MOTOR_DIR_FORWARD);
}


//Should be facing a 90 degree angle of the board at this point so our problem is simpler
//TODO: potentially get rid of LineFound enum
Direction firstDirection = Other;
void savePosition() {
    disableMotor(BOTH_MOTORS);
    enableMotor(BOTH_MOTORS);
    setMotorDirection(BOTH_MOTORS, MOTOR_DIR_FORWARD);
    int curX;
    int curY;

    switch(firstDirection) {
        case Other:
            //be sure to store in this function here the x and/or y values that were found from Calib 1
            //refer to UpdatePosition where I do Odometry_Get(&MyX, &MyY, &MyTheta) and pass in the address of variables and use *to update their values
            //in this case, i would be updating the coordinates of the center cross
            //may also to do the whole resetEncoder count thing

             //TODO add in known offset between line sensors and center turning point of robot
            //replace this with check of IR sensor or we can do the IR sensor check here and at the beginning in order to figure out which way to turn
            //i think i need to do frame transformation for when i eventually reset my odoometry to know where the first X and Y I found are in the new frame
    
            //may want to run through this update 10 more times before doing any logic using the averages since we don't want to be using old data
            //Infrared_Update;
            if (checkLeftIR())
            {
                Odometry_Init(0, 0, SOUTH); //might not be EAST it could be something different but going to assume it is for now
                firstDirection = South;
                //so if actually south, then our new cur Y when we hit a wall is our x distance traveled
            }
            else if (checkCenterIR()) {
                //don't think resetting encoder counts does anything
                //resetLeftEncoderCnt();
                //resetRightEncoderCnt();
                Odometry_Init(0, 0, EAST); // facing East
                firstDirection = East;
            }
            else if (checkRightIR()) {
                Odometry_Init(0, 0, NORTH);
                firstDirection = North;
            }
            break;

            //do I need these?
            //int Origin_x = Odometry_GetX();
            //curY = Odometry_GetY();

            //curX = Odometry_GetX();
            ////1 is a small number basically 0
            //Odometry_Init(curX, 1, SOUTH); // facing SOUTH
            //Origin_y = Odometry_GetY();

            //Serial.print("Origin X is: ");
            //Serial.println(Origin_x);
            //Serial.print("Origin Y is: ");
            //Serial.println(curY);

            // facing South or North, going to guess south then if we hit a wall on left turn then we know we were actually facing north 
            //and we can re-initialize Odom wile keeping track of the Y value we are at
            //always going to assume X is going West-East and Y is South,North
        case South:
            curY = Odometry_GetY();
            Odometry_Init(0, curY, EAST);
            break;
        case East:
            curX = Odometry_GetX();
            Odometry_Init(curX, 0, SOUTH);
            break;
        case North:
            curY = Odometry_GetY();
            Odometry_Init(0, curY, EAST);
            break;
    }
}





//or could just set this to an int i guess and do the compare with the enum


int getDirection() {
    int curTheta = Odometry_GetAngle();
    if (curTheta - EAST < THETATOLERANCE) return East;
    else if (curTheta - NORTH < THETATOLERANCE) return North;
    else if(curTheta - SOUTH < THETATOLERANCE) return South;
    else {
        Serial.println("Something went wrong");
        return Other;
    }
}

void findNextLine()
{
    int curX = Odometry_GetX();
    int curY = Odometry_GetY();
    int curTheta = Odometry_GetAngle();
    int curPos;

    static FNL_States step = FNL_RVRS;

    static const int startDirection = getDirection();

    int goal;
    int32_t absGoal;
    int tolerance;
    int Error;

    switch (step) {
        case FNL_RVRS:
            Reverse();
            //maybe replace this with just a duration time by using the Wait function
            //hopefully break statement doesn't break us out of the whole function. if it goes just take us out
            switch (startDirection) {
            case East:
                goal = -TEN_CENTI;
                curPos = curX;
            case North:
                goal = -TEN_CENTI;
                curPos = curY;
                break;
            case South:
                goal = TEN_CENTI;
                curPos = curY;
                break;
            }
            Error = goal - curPos;   // in counts of 0.0001cm
            tolerance = XYTOLERANCE;
            absGoal = abs(Error);      // in 0.0001 in or 2*pi/16384 radians depending on if looking for x/y or theta=
            if (absGoal < tolerance) step = FNL_TURN;
            break;
        case FNL_TURN:
            switch (startDirection) {
            case East:
                goal = SOUTH;
                SoftRight();
                break;
            case North:
                goal = EAST;
                SoftRight();
                break;
            case South:
                goal = EAST;
                SoftLeft();
                break;
                Error = goal - curTheta;   // in 0.000383 rads
                if (Error >= PI_CUSTOM)Error = Error - TWOPI_CUSTOM;  // -8192 to +8191
                if (Error < -PI_CUSTOM)Error = Error + TWOPI_CUSTOM;  // -8192 to +8191
                tolerance = THETATOLERANCE;
                absGoal = abs(Error);      // in 0.0001 in or 2*pi/16384 radians depending on if looking for x/y or theta
                if (absGoal < tolerance) step = FNL_FWD;
                break;
            }
        case FNL_FWD:
            Forward();  // line detect should occur after this, followed by another lineup function
            break;
    }
}

//in milliseconds
bool Wait(long old_time, long interval)
{
    bool elapsed = false;
    unsigned long currentMillis = millis();
    long timePast = currentMillis - old_time;
    /*Serial.print("timePast is: ");
    Serial.println(timePast);*/
    if (timePast > interval) {
        elapsed = true;
    }
    return elapsed;
}

void getLastBasket()
{
    last_basket = Get_Basket();
    shotTime = millis();
}


/* ================================================== */
/* ----- Utility Getter and Setters for setting states and events in other libraries ----- */
/* ================================================== */

//maybe make one of these for each state machine
void Set_State(int index) {
    //similar to how definition is done above;
    //rState is already a point and this just gives an address to that pointer
    rState = &CSM[index];
}

void Set_Event(int index) {
    //similar to how definition is done above;
    //rState is already a point and this just gives an address to that pointer
    rEvent = Event(index);
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
