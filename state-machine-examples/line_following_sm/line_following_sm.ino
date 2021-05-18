/*(Left,Right) Motors, call LaunchPad_Output (positive logic)
3   1,1     both motors, yellow means go straight
2   1,0     left motor,  green  means turns right
1   0,1     right motor, red    means turn left
0   0,0     both off,    dark   means stop
(Left,Right) Sensors, call LaunchPad_Input (positive logic)
3   1,1     both buttons pushed means on line,
2   1,0     SW2 pushed          means off to right
1   0,1     SW1 pushed          means off to left
0   0,0     neither button      means lost
 */

int LaunchPad_Input();
void turnLeft();
void turnRight();
void straight();
void stopMotion();

// Linked data structure
struct State {
//  String CmdPt;
  void (* CmdPt)(void);
  uint32_t delayTime;              // time to delay in 1ms
  const struct State *next[4]; // Next if 2-bit input is 0-3
};
typedef const struct State State_t;

#define Center     &fsm[0]
#define Left1      &fsm[1]
#define Left2      &fsm[2]
#define Right1     &fsm[3]
#define Right2     &fsm[4]
// student starter code

State_t fsm[5]={
  {&goStraight, 500, { Center, Left1,   Right1,  Center }},  // Center
  {&turnLeft, 500, { Center,  Left2,   Right1,  Center }},  // Left1
  {&goStraight, 500, { Center,  Left1,   Right1,  Center }},  // Left2
  {&turnRight, 500, { Center, Left1,   Right2,  Center }},   // Right1
  {&goStraight, 500, { Center, Left1,   Right1,  Center }}   // Righ2
};

//State_t fsm[5]={
//  {"Straight", 500, { Center, Left1,   Right1,  Center }},  // Center
//  {"Turn Left", 500, { Center,  Left2,   Right1,  Center }},  // Left1
//  {"Straight", 500, { Center,  Left1,   Right1,  Center }},  // Left2
//  {"Turn Right", 500, { Center, Left1,   Right2,  Center }},   // Right1
//  {"Straight", 500, { Center, Left1,   Right1,  Center }}   // Righ2
//};

State_t *Spt;  // pointer to the current state
uint32_t Input;
uint32_t Output;
/*Run FSM continuously
1) Output depends on State (LaunchPad LED)
2) Wait depends on State
3) Input (LaunchPad buttons)
4) Next depends on (Input,State)
 */


const byte LeftSensor = 74; //P1.4
const byte RightSensor = 73;  //P1.1
const byte LeftMotor = 75; //P2.0
const byte RightMotor = 76; //P2.1
const byte BothMotors = 77; //P2.2


void setup()
{
  Serial.begin(9600);
  pinMode(LeftMotor, OUTPUT); 
  pinMode(RightMotor, OUTPUT); 
  pinMode(BothMotors, OUTPUT); 
  pinMode(LeftSensor, INPUT_PULLUP);
  pinMode(RightSensor, INPUT_PULLUP);
}
 
void loop()
{ 
  uint32_t heart=0;
  Spt = Center;
  while(1){
    Serial.print("Output is ");
    Spt->CmdPt();            // set output from FSM
    delay(Spt->delayTime);   // wait
    Input = LaunchPad_Input();    // read sensors
    Serial.print("Input is ");
    Serial.println(Input);
    Spt = Spt->next[Input];       // next depends on input and state
//    Serial.println(Spt->next[Input]);
//    heart = heart^1;
//    LaunchPad_LED(heart);         // optional, debugging heartbeat
  }
}

// Color    LED(s) Port2
// dark     ---    0
// red      R--    0x01
// blue     --B    0x04
// green    -G-    0x02
// yellow   RG-    0x03
// sky blue -GB    0x06
// white    RGB    0x07
// pink     R-B    0x05


int LaunchPad_Input()
{
  int LeftSensorValue = digitalRead(LeftSensor);
  int RightSensorValue = digitalRead(RightSensor)<<1;

  int result = (LeftSensorValue | RightSensorValue);
  
  return result;
}

//GREEN
void turnLeft(){
  digitalWrite(RightMotor,HIGH);
  digitalWrite(LeftMotor,LOW);
  digitalWrite(BothMotors,LOW);
  delay(10);
  Serial.println("Turn Left");
}

//RED
void turnRight(){
  digitalWrite(RightMotor,LOW);
  digitalWrite(LeftMotor,HIGH);
  digitalWrite(BothMotors,LOW);
  delay(10);
  Serial.println("Turn Left");
  
}

//BLUE
void goStraight(){
  digitalWrite(RightMotor,LOW);
  digitalWrite(LeftMotor,LOW);
  digitalWrite(BothMotors,HIGH);
  delay(10);
  Serial.println("Turn Left");
}

//YELLOW
void stopMotion(){
  digitalWrite(RightMotor,HIGH);
  digitalWrite(LeftMotor,HIGH);
  digitalWrite(BothMotors,LOW);
  delay(10);
  Serial.println("Turn Left");
}
