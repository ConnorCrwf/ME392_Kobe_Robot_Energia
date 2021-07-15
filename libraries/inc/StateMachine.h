//Why don't we include all this in just one header. Like combine States and StateMachine

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "States.h"  //because of STyp definition
//#include "States_debug.h"

/* ================================================== */
/* ----- CALIB 1 STATE ----- */
/* ================================================== */


//declared extern here, defined in the source file, and then used in a different source file
extern STyp CSM[7];

//good to keep these in this file since CSM is defined here
//this gets the 0th index of the array of structs so it gets the first struct
#define Init_Cal &CSM[INIT_CAL]
#define Fwd_Cal &CSM[FWD_CAL]
#define Lineup_Cal &CSM[LINEUP_CAL]
#define FindNextLine &CSM[FIND_NEXT_LINE]
#define Lineup_Cal2 &CSM[LINEUP_CAL2]
#define Complete_Cal &CSM[COMPLETE_CAL]
#define Collision &CSM[COLLISION]


enum CSTATE {
    INIT_CAL, //start going fwd
    FWD_CAL,
    LINEUP_CAL,
    FIND_NEXT_LINE,
    LINEUP_CAL2,
    COMPLETE_CAL,
    COLLISION
};





/* ================================================== */
/* ----- CALIB 2 STATE ----- */
/* ================================================== */

//extern STyp C2SM[6];
//
//#define Init_Cal2 &C2SM[INIT_CAL2]
//#define Fwd_Cal2 &C2SM[FWD_CAL2]
//#define Rvrs1_Cal2 &C2SM[RVRS1_CAL2]
//#define Lineup_Cal2 &C2SM[LINEUP_CAL2]
//#define Rvrs2_Cal2 &C2SM[RVRS2_CAL2]
//#define Complete_Cal2 &C2SM[COMPLETE_CAL2]

//enum C2STATE {
//    INIT_CAL2, //rotate
//    FWD_CAL2,  //go forward
//    RVRS1_CAL2,
//    LINEUP_CAL2,
//    RVRS2_CAL2,
//    COMPLETE_CAL2
//};


/* ================================================== */
/* ----- NAVIGATION STATE ----- */
/* ================================================== */


extern STyp NSM[4];

#define Init1_Nav &NSM[INIT1_NAV]
#define Init2_Nav &NSM[INIT2_NAV]
#define Init3_Nav &NSM[INIT3_NAV]
#define Complete_Nav &NSM[COMPLETE_NAV]

enum NSTATE {
    INIT1_NAV,
    INIT2_NAV,
    INIT3_NAV,
    COMPLETE_NAV
};


/* ================================================== */
/* ----- SHOOTING STATE ----- */
/* ================================================== */


extern STyp SSM[3];

#define Init_Shooting &SSM[INIT_SHOOTING]
#define Check_Shot &SSM[CHECK_SHOT]
#define Complete_Shooting &SSM[COMPLETE_SHOOTING]

enum SSTATE {
    INIT_SHOOTING,
    CHECK_SHOT,  //Complete Shooting if IR sensor changes. Turn left or right if IR sensor didn't change
    COMPLETE_SHOOTING
};


/* ================================================== */
/* ----- EVENTS ----- */
/* ================================================== */




/* ================================================== */
/* ----- MISC ----- */
/* ================================================== */


//if want to change rState, rStage, or rEvent in this file, then you must do an extern of the variable
//rEvent for example looks like extern Event rEvent;

#endif

// LEFTOVER CODE, incorporate into StateMachine_debug.h

//extern STyp CSM[2];
//#define Init_Cal &CSM[INIT_CAL]
//#define Fwd_Cal &CSM[FWD_CAL]
//
//enum CSTATE {
//    INIT_CAL,
//    FWD_CAL
//};
