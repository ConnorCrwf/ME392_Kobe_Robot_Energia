#include "Infrared.h"

#define IR_DEQUE_SIZE 10

#define IR_LEFT_PIN A16
#define IR_CENTER_PIN A14
#define IR_RIGHT_PIN A17

Deque<int> IRData_Left = Deque<int>(IR_DEQUE_SIZE);  //basket 0
Deque<int> IRData_Center = Deque<int>(IR_DEQUE_SIZE);  //basket 1
Deque<int> IRData_Right = Deque<int>(IR_DEQUE_SIZE);  //basket 2

void setupIRsensors() {
    for (int i = 0; i < IR_DEQUE_SIZE + 1; i++) {
        IRData_Left.push_back(1000);
        IRData_Center.push_back(1000);
        IRData_Right.push_back(1000);
    }
}

int IRData_LeftAve = 1000;
int IRData_CenterAve = 1000;
int IRData_RightAve = 1000;

//TODO try making deque a pointer again
void Calc_IR_Ave(Deque<int> IRData, int* IRData_Ave, int pin) {
    //
    //read IR sensor,
    //TODO fix this
    uint32_t IRval = analogRead(pin);
    //Be careful with this
    delay(100);
    /*Serial.print("IRval is : ");
    Serial.println(IRval);*/
    //even though it's initialized to a certain size, count is only concerned with what has been added to it after intilization
    //could also do (*IRData).count because otherwise it would do *IRData.count
    int sizeIRData = IRData.count();
    /*Serial.print("Size of IRData is : ");
    Serial.println(sizeIRData);*/
    IRData.pop_front();
    IRData.push_back(IRval);

    int* data = IRData.data_array();
    int sum;
    for (int i = 0; i < IR_DEQUE_SIZE + 1; i++) {
        sum += data[i];
    }
    //converting to a moving average of maybe the last 10 values.
    *IRData_Ave = sum / sizeIRData;

}

int Infrared_GetLeft(void) {
    return IRData_LeftAve;
}

int Infrared_GetRight(void) {
    return IRData_RightAve;
}

int Infrared_GetCenter(void) {
    return IRData_CenterAve;
}

void Infrared_Update(void) {
    Calc_IR_Ave(IRData_Left, &IRData_LeftAve, IR_LEFT_PIN);
    Calc_IR_Ave(IRData_Center, &IRData_CenterAve, IR_CENTER_PIN);
    Calc_IR_Ave(IRData_Right, &IRData_RightAve, IR_RIGHT_PIN);
}

int Get_Basket() {
    //here we are sending pointers to this function since that's what it wants. definition of that function will always be something like int* or String* then variable name
    Calc_IR_Ave(IRData_Left , &IRData_LeftAve, IR_LEFT_PIN);
    //Calc_IR_Ave(&IRData_Center, &IRData_CenterAve, IR_CENTER_PIN);
    //Calc_IR_Ave(&IRData_Right, &IRData_RightAve, IR_RIGHT_PIN);
    Serial.print("IR Left Average is : ");
    //maybe include in here a Getter function for best practice
    Serial.println(IRData_LeftAve);
    if (IRData_LeftAve < 500) return 0;
    else if (IRData_CenterAve < 500) return 1;
    else if (IRData_RightAve < 500) return 2;
    else return 3;
}

bool checkCenterIR() {
    if (IRData_CenterAve < 500) return true;
    else return false;
}

bool checkLeftIR() {
    if (IRData_LeftAve < 500) return true;
    else return false;
}

bool checkRightIR() {
    if (IRData_RightAve < 500) return true;
    else return false;
}
