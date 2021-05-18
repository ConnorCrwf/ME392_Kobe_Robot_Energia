#ifndef STATES_H_
#define STATES_H_

//TODO split up into a Command Pointer library, a closing function lib, an event check lib

#define TEN_CENTI 100000
#define FIVE_CENTI 50000

#include <stdbool.h>
#include <stdint.h>
#include "Energia.h"
//#include "StateEvents.h"

extern int Origin_x;
extern int Origin_y;

//maybe move these to State Machine library
enum StateMachineStage {
    CALIB,  //whole point of this stage is to find the center line and 0 odometry at the center point 
    // depending on if the IR sensor is picked up the most by the left, right, or center will dictate where we find the x or y part of this point
    NAVIGATION,
    SHOOTING
};

struct OdomGoal{
    const String direction;
    int32_t goaVals[3]; 
    //to do logic on this, can use '==' since it's a char
    //const char direction[];
};

typedef const struct OdomGoal OdomGoalTyp;

// Forward declaration for the state type that all stages use (this may not be the case in the future)
//can use different events with this struct type. if need to use 5 events instead of 4, best to just make another typdef below and call it like
//typedef const struct State NewTyp;
struct State{
    // can I use this with dot now instead of -> ?
    //to do any logic on this must use strcmp
    const String stateName;
    //struct array that shows us what state to go to next. So all this does is pass an index to 
    //TODO how do I declare this to use or not to use an argument. Is that like a two cases declaration?
    void (*CmdPt)(void); //what if I want to maybe pass a paramter in to CmtPt depending on if i want to track odom distance to change state after set distance?
    void (*ClosingPt)(void);
    OdomGoalTyp curGoal;              //if I didn't make this a pointer, could i just acces its goal and goalVals elements with dot instead of ->?
    //size is number of inputs
    //this integer array can be accessed with enums; basically this element of the State struct has a size of 4 since there are 4 possible inputs
    //if rEvent is INIT for example, then that returns/evaluates to 0 since that is the first position of the enum CalibrationEvent
    //if rEvent is RVRS for example, then that evaluates to 1
    //based on the definition that is stored in the specific 
    const struct State *Next[5];
    //Where is the connection between this integer array and the the fact that second position of the RobotState struct is also an integer array rCalEvent?
    //const uint8_t next_state_by_event[4];
    //an address to a function is the third part of this struct and then it is de-referenced here to in order to be run
};

typedef const struct State STyp;

//Main State Machine Program functions
void State_Init();
void State_Action();
void State_Delay();
void State_Event();
void State_Next();

//Utility Functions
//void Set_State(STyp* state);
void Set_State(int index);
void Set_Event(int index);
bool Wait(long interval);

//Command Pointer Functions, move these into their own Action library to simplify code reading
void advance();
void lineup();
void findNextLine();






//Closing Functions
void closingFnc();
void savePosition();
int getDirection();
void getLastBasket();


//different than defined values
enum Direction {
    North,
    East,
    South,
    Other
};

enum FNL_States {
    FNL_RVRS,
    FNL_TURN,
    FNL_FWD
};

#endif

// Global Variables: Be very careful with these
//extern enum CalibrationEvent rCalEvent;