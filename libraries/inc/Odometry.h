#ifndef _ODOMETRY_H_
#define _ODOMETRY_H_

//#ifdef __cplusplus
//extern "C" {
//#endif

#include <stdint.h>

#define N 360     ///< counts/rotation, just one edge of one tach
#define D 70000   ///< wheel diameter 0.0001cm
#define W 140000  ///< wheel base 0.0001 cm
#define w2 (W/2) // used for rounding
#define C 219910  ///< wheel circumference 0.0001cm
#define PI_CUSTOM 8192   ///< representation of pi radians
#define TWOPI_CUSTOM (2*PI_CUSTOM) ///< 6.28...
//each degree is a value of 45 or 4096/90
#define NORTH 3900   ///< direction that is north was 4096
#define EAST  0      ///< direction that is east
#define SOUTH -3900  ///< direction that is south, was -4096
#define WEST -7800   ///< direction that is west
#define XYTOLERANCE 1000      ///< tolerance in x,y goal seeking, 0.1 cm was 10,000 for 1 cm
#define THETATOLERANCE (4096/180)  ///< tolerance in angle goal seeking, 0.25 deg, was 4096/90 for 1 degree

void Odometry_Init(int32_t initx, int32_t inity, int32_t initTheta);
void Odometry_Update(int32_t LCount, int32_t RCount);
void Odometry_Get(int32_t* x, int32_t* y, int32_t* theta);
int32_t Odometry_GetX(void);
int32_t Odometry_GetY(void);
int32_t Odometry_GetAngle(void);
void UpdatePosition(int32_t TotalLeftSteps, int32_t TotalRightSteps);
//Figure out how to bring these back in
void SoftLeft(void);
void SoftRight(void);
//void SoftLeftUntilTh(int32_t desiredTh);
void Forward();
void Reverse();

//#ifdef __cplusplus
//}
//#endif

#endif

