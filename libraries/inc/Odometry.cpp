#include <stdint.h>
#include <stdio.h>
#include "fixed.h"; //a C header, so wrap it in extern "C" 
#include "Odometry.h"
#include <math.h>
#include "SimpleRSLK.h"
//#include "Tachometer.h"
//#include "..\inc\Bump.h"
//#include "..\inc\blinker.h"

int32_t Robotx, Roboty; // position in 0.0001cm
int32_t Robottheta;    // direction units 2*pi/16384 radians (-pi to +pi)
// e.g., 90 degrees (pi/2 radians) is 4096
// private variables
int32_t Lr;     // LCount*c/n the arc distance traveled by the left wheel (0.0001cm)
int32_t Rr;     // RCount*c/n the arc distance traveled by the right wheel (0.0001cm)
int32_t L;      // distance from left wheel to pivot point
int32_t R;      // distance from right wheel to pivot point
int32_t dtheta; // change in direction 2*pi/16384 radians (-pi to +pi)
int32_t dz;     // change in distance 0.0001



//Other Declarations
//Left Tach
//uint16_t LeftTach;             // tachometer period of left wheel (number of 0.0833 usec cycles to rotate 1/360 of a wheel rotation)
//enum TachDirection LeftDir;    // direction of left rotation (FORWARD, STOPPED, REVERSE)
int32_t LeftSteps;             // number of tachometer steps of left wheel (units of 220/360 = 0.61 mm traveled)
int32_t LastLeftSteps;         // number of tachometer steps of left wheel (units of 220/360 = 0.61 mm traveled)
int32_t TotalLeftSteps;        // number of tachometer steps of left wheel (units of 220/360 = 0.61 mm traveled)

//Right Tach
//uint16_t RightTach;            // tachometer period of right wheel (number of 0.0833 usec cycles to rotate 1/360 of a wheel rotation)
//enum TachDirection RightDir;   // direction of right rotation (FORWARD, STOPPED, REVERSE)
int32_t RightSteps;            // number of tachometer steps of right wheel (units of 220/360 = 0.61 mm traveled)
int32_t LastRightSteps;        // number of tachometer steps of right wheel (units of 220/360 = 0.61 mm traveled)
int32_t TotalRightSteps;       // number of tachometer steps of right wheel (units of 220/360 = 0.61 mm traveled)

//Odom Get Stuff
int32_t MyX, MyY;               // position in 0.0001cm
// e.g., 90 degrees (pi/2 radians) is 4096
int32_t MyTheta;               // direction units 2*pi/16384 radians (-pi to +pi)

uint32_t MotorFast = 4000; // PWM for fast motions, out of 15000
uint32_t MotorSlow = 2005; // PWM for slow motions, out of 15000

void Odometry_Init(int32_t initx, int32_t inity, int32_t initTheta) {
    Robotx = initx;
    Roboty = inity;
    Robottheta = initTheta;
    int a = getEncoderLeftCnt();
    int b = getEncoderRightCnt();
    /*Serial.println(a);
    Serial.println(b);*/
}

void Odometry_Update(int32_t LCount, int32_t RCount)
{
    /*Serial.print("Robot is: ");
    Serial.println(Robottheta);*/
    /*Serial.println(LCount);
    Serial.println(RCount);*/
    int32_t L2;
    int32_t absLr, absRr;
    Lr = (LCount * C) / N;      // 0.0001cm
    Rr = (RCount * C) / N;      // 0.0001cm
    if (Lr >= 0) {
        absLr = Lr;
    }
    else {
        absLr = -Lr;
    }
    if (Rr >= 0) {
        absRr = Rr;
    }
    else {
        absRr = -Rr;
    }
    if (Lr == Rr) {
            //Serial.println("Going Straight");
        dtheta = 0;        // straight
        dz = Lr / 2;
        //    Serial.print("dtheta: ");
        //    Serial.println(dtheta);
    }
    else if (absLr < absRr) {
        //Serial.println("Turning Left");
        // turning left?
        if (Lr == 0) {       // Lr is 0
            L = 0;           // left wheel stopped
            dtheta = (2608 * Rr + w2) / W;
            dz = (dtheta * W + 5215) / 10430;  // 0.0001cm, approximation
        }
        else {
            L = (W * Lr) / (Rr - Lr);        // 0.0001cm
            L2 = L / 2;
            if (L > 0) {
                dtheta = (2608 * Lr + L2) / L;   // 2608=16384/2/pi
            }
            else {
                dtheta = (2608 * Lr - L2) / L;   // 2608=16384/2/pi
            }
            dz = (dtheta * (L + w2) + 2608) / 5215;  // 0.0001cm, approximation
      //    dtheta = (10000*Lr)/L;        // 0.0001 radians
      //    dz = (dtheta*(L+w/2))/20000;  // 0.0001cm, approximation
        }
    }
    else {
        //Serial.println("Turning Right");
        if (Rr == 0) {
            L = 0;          // right wheel stopped
            dtheta = (-2608 * Lr + w2) / W;
            dz = (-dtheta * W + 5215) / 10430;       // 0.0001cm, approximation
        }
        else {
            R = (W * Rr) / (Lr - Rr);        // 0.001cm
            dtheta = (-2608 * Rr) / R;       // 2*pi/16384 radians
            dz = (-dtheta * (R + w2) + 2608) / 5215; // 0.0001cm, approximation
        }
    }
    //  or  dz = (tan(dtheta/2)*(L+w/2))/10000    (0.0001cm) more accurate
    Robotx = Robotx + (dz * fixed_cos2(Robottheta)) / 65536;  // 0.0001cm
    Roboty = Roboty + (dz * fixed_sin2(Robottheta)) / 65536;  // 0.0001cm  first part of move
    Robottheta += dtheta;               // 0.001 radians
    /*Serial.print("dtheta: ");
    Serial.println(dtheta);
    Serial.print("Robot is: ");
    Serial.println(Robottheta);*/
    if (Robottheta >= PI_CUSTOM)Robottheta = Robottheta - TWOPI_CUSTOM;  //-8192 to +8191
    if (Robottheta < -PI_CUSTOM)Robottheta = Robottheta + TWOPI_CUSTOM;
    Robotx = Robotx + (dz * fixed_cos2(Robottheta)) / 65536;  // 0.0001cm
    Roboty = Roboty + (dz * fixed_sin2(Robottheta)) / 65536;  // 0.0001cm  second part of move
}


void UpdatePosition(int32_t TotalLeftSteps, int32_t TotalRightSteps) {
    //Tachometer_Get(&TotalLeftSteps, &TotalRightSteps);
    /*Serial.println(TotalLeftSteps);
    Serial.println(TotalRightSteps);*/
    LeftSteps = TotalLeftSteps - LastLeftSteps;
    RightSteps = TotalRightSteps - LastRightSteps;
    /*Serial.println(LeftSteps);
    Serial.println(RightSteps);*/
    Odometry_Update(LeftSteps, RightSteps);
    Odometry_Get(&MyX, &MyY, &MyTheta);
    LastLeftSteps = TotalLeftSteps;
    LastRightSteps = TotalRightSteps;
    //Serial.println(MyTheta * .000383);
}

//What is the point of this if we already have Robotx, Roboty, and Robottheta.
//I think it's one way we could update variables for odom that exist in another file. Although that is not good practice
//Figure out how to make this work with C++. Has to do with strong or weakly typed pointers I think
//if this doesn't work for C++, why don't I get any errors when I include it directly like I did in the first OdomTest
void Odometry_Get(int32_t* x, int32_t* y, int32_t* theta) {
    *x = Robotx;
    *y = Roboty;
    *theta = Robottheta;
    //Serial.println(Robottheta);
}

int32_t Odometry_GetX(void) {
    return Robotx;
}
int32_t Odometry_GetY(void) {
    return Roboty;
}
int32_t Odometry_GetAngle(void) {
    return Robottheta;
}



void SoftLeft(void) {
    setMotorSpeed(RIGHT_MOTOR, MotorSlow);
    setMotorSpeed(LEFT_MOTOR, 0);
}

void SoftRight(void) {
    setMotorSpeed(LEFT_MOTOR, MotorSlow);
    setMotorSpeed(RIGHT_MOTOR, 0);
}


/*
void SoftLeftUntilTh(int32_t desiredTh) {
    int32_t goal = 4096;  // in 2*pi/16384 radians, start it out high here so that we get into while loop. could also do a do while loop
    int32_t badCount = 10;
    int32_t lastgoal = abs(desiredTh - MyTheta);  // in 2*pi/16384 radians
    SoftLeft();
    long previousMillis = 0;
    long interval = 40;
    while (goal > THETATOLERANCE)
    {
        //May want to look into putting this while loop in main code and just returning goal that way we don't have to call Update Position in here as well.
        //Probably not that big a deal though
        unsigned long currentMillis = millis();
        //without accounting for setup time offset, it will just call it sooner the first time through
        if (currentMillis - previousMillis > interval)
        {
            UpdatePosition(getEncoderLeftCnt(), getEncoderRightCnt());
            previousMillis = currentMillis;
            Serial.println(MyTheta*.000383);
        }
        Error = desiredTh - MyTheta;   // in 2*pi/16384 radians
        if (Error >= PI_CUSTOM)Error = Error - TWOPI_CUSTOM;  // -8192 to +8191
        if (Error < -PI_CUSTOM)Error = Error + TWOPI_CUSTOM;  // -8192 to +8191
        goal = abs(Error);           // in 2*pi/16384 radians
        //Serial.println(goal);
        if (goal > lastgoal)
        {         // missed it, going wrong way??
            badCount--;
            if (badCount <= 0) {
                //Serial.println("Wrong way");
            }
            lastgoal = goal;
        }
    }
    disableMotor(BOTH_MOTORS);
}
*/

void Forward(void) {
    setMotorDirection(BOTH_MOTORS, MOTOR_DIR_FORWARD);
    setMotorSpeed(BOTH_MOTORS, MotorSlow);
}

void Reverse(void) {
    setMotorDirection(BOTH_MOTORS, MOTOR_DIR_BACKWARD);
    setMotorSpeed(BOTH_MOTORS, MotorSlow);
}