#ifndef STATE_EVENTS_H_
#define STATE_EVENTS_H_

  
#include "States.h"  // because of STyp being defined in States.h

//maybe split this back up into Cal Nav and Shooting Event but for right now it's not that big

//only have inputs that are used across the board or within this particular stage
//some stages may use structures that don't need 4 inputs
//may bring in IR sensor, but for now it's not used all that much so just going to handle it with an if statement. This may change once I find out more about my SM structure
enum Event {
    NOTHING,
    BUMP,
    LINE_DETECT,
    GOAL_REACHED
};

enum ShootingEvent {
    SHOT_NOTHING,
    SHOT_BUMP,
    SHOT_MISSED,
    SHOT_MADE
};

void checkBump(Event& event);
void checkOdomGoal(STyp* state, Event& event, bool debug);
void checkCenterIRGoal(int* angle, Event& event);
void checkLine(Event& event, bool debug);
void checkShot(ShootingEvent& event, int& last_basket, long& shotTime);

void updateLineDataHistory(uint32_t linePos);
int getNonZeroCount();

#endif