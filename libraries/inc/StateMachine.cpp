#include "Odometry.h"
#include "SimpleRSLK.h"
#include "StateMachine.h"
#include "Shooting.h"
#include "States.h"
//#include "States_debug.h"

/* ================================================== */
/* ----- Initialize the state machines associated with the 4 stages of this program ----- */
/* ================================================== */

//this is an array of structs that is of size 5
//TODO turn this into Localization instead of Calibration
//Calibration State Machine
//if add a new input, only have to worry about adding an additional memeber at end
//Events are           NOTHING, BUMP, LINE_DETECT, GOAL_REACHED (only 4 inputs here)


STyp CSM[7] = {
    //{"Init_Cal", &SoftLeft, &closingFnc, {"Theta",{0,0,NORTH}}, {Init_Cal, Collision, Lineup_Cal, Lineup_Cal}},
    {"Init_Cal", &closingFnc, &closingFnc, {"None",{0,0,0}}, {Init_Cal, Collision, Init_Cal, Fwd_Cal}},
    {"Fwd_Cal", &Forward, &closingFnc, {"None",{0,0,0}}, {Fwd_Cal, Collision, Lineup_Cal, Fwd_Cal} }, 
    //rotate handles the transition to Complete_Cal and Complete_Cal2 depending on the stage
    //rEvent GOAL_REACHED is set in lineup so State_Event when called next doesn't really do anything
    {"Lineup_Cal", &lineup, &savePosition, {"None",{0,0,0}}, {Lineup_Cal, Collision, Lineup_Cal, FindNextLine} },
    {"FindNextLine", &findNextLine, &closingFnc, {"None",{0,0,0}}, {FindNextLine, Collision, Lineup_Cal2, FindNextLine}},
    {"Lineup_Cal2", &lineup, &savePosition, {"None",{0,0,0}}, {Lineup_Cal2, Collision, Lineup_Cal2, Complete_Cal} },
    //Event that brings it into Complete_Cal is set in State_Event
    //Doesn't matter what next rEvent unless it's a bump, it will stay in Complete_Cal
    {"Complete_Cal", &advance, &closingFnc, {"None",{0,0,0}}, {Complete_Cal, Collision,  Complete_Cal, Complete_Cal}},  //change rstage
    //Goes at the end here since this is a state that should only occur if something messes up
    //TODO Change Reverse to after a bump detect we should turn a specific amount. 
    //also we may be going a little in x and a little in y, so make a custom reverse function that returns GOAL_REACHED after a specific amoiunt of time
    {"Collision", &Reverse, &closingFnc, {"None",{0,0,0}}, {Collision, Init_Cal, Collision, Init_Cal}}
};




//Events are           NOTHING, BUMP, LINE_DETECT, GOAL_REACHED (only 4 inputs here)
//STyp C2SM[6] = {
//    {"Init_Cal2", &SoftRight, &closingFnc, {"Theta",{0,0,SOUTH}}, {Init_Cal2, Init_Cal2, Init_Cal2, Fwd_Cal2}},
//    {"Fwd_Cal2", &Forward, &closingFnc, {"None",{0,0,0}}, {Fwd_Cal2, Rvrs1_Cal2, Lineup_Cal2, Rvrs2_Cal2}},
//    {"Rvrs1_Cal2", &Reverse, &closingFnc, {"Y",{0,1.5 * TEN_CENTI,0}}, {Rvrs1_Cal2, Init_Cal2, Lineup_Cal2, Init_Cal2}},
//    {"Lineup_Cal2", &lineup, &saveY, {"None",{0,0,0}}, {Lineup_Cal2, Rvrs1_Cal2, Lineup_Cal2, Rvrs2_Cal2} },
//    {"Rvrs2_Cal2", &Reverse, &closingFnc, {"Y",{0,1.5 * TEN_CENTI,0}}, {Rvrs2_Cal2, Rvrs2_Cal2, Rvrs2_Cal2, Complete_Cal2} },
//    {"Complete_Cal2", &advance, &closingFnc, {"None",{0,0,0}}, {Complete_Cal2, Complete_Cal2,  Complete_Cal2, Complete_Cal2} }  //change rstage
//};

//Events are           NOTHING, BUMP, LINE_DETECT, GOAL_REACHED (only 4 inputs here)

STyp NSM[4] = {
    {"Init1_Nav", &Forward, &closingFnc, {"Y",{0,Origin_y,0}}, {Init1_Nav, Init1_Nav, Init1_Nav, Init2_Nav}},
    {"Init2_Nav", &SoftLeft, &closingFnc, {"Theta",{0,0,EAST}}, {Init2_Nav, Init2_Nav, Init2_Nav, Init3_Nav}},
    {"Init3_Nav", &Forward, &closingFnc, {"X",{Origin_x,0,0}}, {Init3_Nav, Init3_Nav, Init3_Nav, Complete_Nav}},
    {"Complete_Nav", &advance, &closingFnc, {"None",{0,0,0}}, {Complete_Nav, Complete_Nav,  Complete_Nav, Complete_Nav} }  //change rstage
};


//Events are           NOTHING, BUMP, SHOT_MISSED, SHOT_MADE. these are sEvents not rEvents, though.

STyp SSM[3] = {
    {"Init_Shooting", &Shoot, &getLastBasket, {"None",{0,0,0}}, {Check_Shot, Check_Shot, Check_Shot, Check_Shot}},
    {"Check_Shot", &dummy, &closingFnc, {"None",{0,0,0}}, {Check_Shot, Check_Shot, Init_Shooting, Complete_Shooting}},
    {"Complete_Shooting", &advance, &closingFnc, {"None",{0,0,0}}, {Complete_Shooting, Complete_Shooting,  Complete_Shooting, Complete_Shooting} }  //change rstage
};



// LEFTOVER CODE, incorporate into StateMachine_debug.h

//Simple Calibration Stage implementation
//STyp CSM[2] = {
//    {"Init_Cal", &closingFnc, &closingFnc, {"None",{0,0,0}}, {Init_Cal,Fwd_Cal}},
//    {"Fwd_Cal",  &closingFnc, &closingFnc, {"None",{0,0,0}}, {Init_Cal, Fwd_Cal}}
//};

/*
        count++;
        Serial.println(count);
        if (count > 4) {
            //rState = Complete_Cal;
            //this allows me to not have to use externs for rState, which is generally bad practice
            //these are the externs that were originally in Calib1.h
            //extern STyp* rState;
            //extern STyp CSM[4];
            Set_State(COMPLETE_CAL1);
        }
        */