#ifndef IR_H_
#define IR_H_

#include "Deque.h"



void setupIRsensors();

void Calc_IR_Ave(Deque<int> IRData, int* IRData_Ave);
void Infrared_Update(void);

int Infrared_GetLeft(void);
int Infrared_GetRight(void);
int Infrared_GetCenter(void);

//0 - Left, 1 - Center, 2 - Right
int Get_Basket();

bool checkCenterIR();
bool checkLeftIR();
bool checkRightIR()


#endif
