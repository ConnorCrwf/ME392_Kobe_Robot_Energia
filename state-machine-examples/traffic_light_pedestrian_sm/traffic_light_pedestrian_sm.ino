
// east facing red light connected to P4.5
// east facing yellow light connected to P4.4
// east facing green light connected to P4.3
// north facing red light connected to P4.2
// north facing yellow light connected to P4.1
// north facing green light connected to P4.0
// pedestrian detector connected to P5.2 (1=pedestrian present)
// north facing car detector connected to P5.1 (1=car present)
// east facing car detector connected to P5.0 (1=car present)
// pedestrian red connected to P2.0
// pedestrian green connected to P2.1

// SENSOR is set to P5IN and LIGHT is set to P4OUT, making the code more readable and WALK_LIGHT is P2OUT
#define WALK_RED P2_0
#define WALK_GREEN P2_1
#define SOUTH_GREEN P4_0
#define SOUTH_YELLOW P4_1
#define SOUTH_RED P4_2
#define WEST_GREEN P4_3
#define WEST_YELLOW P4_4
#define WEST_RED P4_5

#define EAST_DETECT P5_0
#define NORTH_DETECT P5_1
#define PEDESTRIAN_DETECT P5_2


// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void Port2_Init(void);
void Port4_Init(void);
void Port5_Init(void);
unsigned long Sensor_Input(void);
void goSouth(void);
void waitSouth(void);
void goWest(void);
void waitWest(void);
void Walk(void);
void walkHurryOff(void);
void walkHurryOn(void);

// Linked data structure
struct State {
    void (*CmdPt)(void);   // output function
    uint32_t Time;
    const struct State *Next[8];
    char Name[10];
};
typedef const struct State STyp;
#define goS   &FSM[0]
#define waitS &FSM[1]
#define goW   &FSM[2]
#define waitW &FSM[3]
#define walk &FSM[4]
#define wh1off &FSM[5]
#define wh1on &FSM[6]
#define wh2off &FSM[7]
#define wh2on &FSM[8]

STyp FSM[9] = {
/*goS */ {&goSouth,100,{goS,waitS,goS,waitS,waitS,waitS,waitS,waitS},"goS"},
/*waitS */ {&waitSouth, 100,{goW,goW,goW,goW,walk,walk,walk,goW},"waitS"},
/*goW */ {&goWest,100,{goW,goW,waitW,waitW,waitW,waitW,waitW,waitW},"goW"},
/*waitW */ {&waitWest, 100,{goS,goS,goS,goS,walk,walk,walk,walk},"waitW"},
/*walk */ {&Walk, 100,{walk,wh1off,wh1off,wh1off,walk,wh1off,wh1off,wh1off},"walk"},
/*wh1off */ {&walkHurryOff, 100,{wh1on,wh1on,wh1on,wh1on,wh1on,wh1on,wh1on,wh1on},"wh1off"},
/*wh1on */ {&walkHurryOn, 100,{wh2off,wh2off,wh2off,wh2off,wh2off,wh2off,wh2off,wh2off},"wh1on"},
/*wh2off */ {&walkHurryOff, 100,{wh2on,wh2on,wh2on,wh2on,wh2on,wh2on,wh2on,wh2on},"wh2off"},
/*wh2on*/ {&walkHurryOn, 100,{walk,goW,goS,goS,walk,goW,goS,goS},"wh2on"}
};

// ***** 3. Subroutines Section *****
uint8_t Input;
STyp *S;  // state pointer
char stateName[10];

void setup() {
  Serial.begin(9600);
  Port2_Init();   // initialize P2.2-0 to output
  Port4_Init();   // initialize P4.5-0 to output
  Port5_Init();   // initialize P5.5-0 to input
  S = goS;
}

void loop(){
  S->CmdPt();           // call output function
  delay(S->Time);
  Input = Sensor_Input();       // input can be 0-7
  S = S->Next[Input];
  strcpy(stateName,S->Name);
  Serial.print("State is");
  Serial.println(stateName);
  }


void Port2_Init(void) {
  pinMode(EAST_DETECT, INPUT);
  pinMode(NORTH_DETECT, INPUT);
  pinMode(PEDESTRIAN_DETECT, INPUT);
}

void Port4_Init(void) {
  // initialize P4.5-P4.0 and make them outputs
  pinMode(SOUTH_GREEN, OUTPUT); 
  pinMode(SOUTH_YELLOW, OUTPUT); 
  pinMode(SOUTH_RED, OUTPUT); 
  pinMode(WEST_GREEN, OUTPUT); 
  pinMode(WEST_YELLOW, OUTPUT); 
  pinMode(WEST_RED, OUTPUT); 
}

void Port5_Init(void) {
  pinMode(WEST_YELLOW, OUTPUT); 
  pinMode(WEST_RED, OUTPUT); 
}

unsigned long Sensor_Input(void) {
  int eastDetect = digitalRead(EAST_DETECT);
  int northDetect = digitalRead(NORTH_DETECT)<<1;
  int pedDetect = digitalRead(PEDESTRIAN_DETECT)<<2;
  
  int result = (eastDetect | northDetect | pedDetect);
  
  return result;
}

void clearPins(void) {
  digitalWrite(WALK_RED,LOW);
  digitalWrite(WALK_GREEN,LOW);
  digitalWrite(SOUTH_GREEN,LOW);
  digitalWrite(SOUTH_YELLOW,LOW);
  digitalWrite(SOUTH_RED,LOW);
  digitalWrite(WEST_GREEN,LOW);
  digitalWrite(WEST_YELLOW,LOW);
  digitalWrite(WEST_RED,LOW);
}

void goSouth(void) {
    clearPins();
    digitalWrite(WALK_RED,HIGH);
    digitalWrite(SOUTH_GREEN,HIGH);
    digitalWrite(WEST_RED,HIGH);
}

void waitSouth(void) {
    clearPins();
    digitalWrite(WALK_RED,HIGH);
    digitalWrite(SOUTH_YELLOW,HIGH);
    digitalWrite(WEST_RED,HIGH);
}

void goWest(void) {
    clearPins();
    digitalWrite(WALK_RED,HIGH);
    digitalWrite(SOUTH_RED,HIGH);
    digitalWrite(WEST_GREEN,HIGH);
}

void waitWest(void) {
    digitalWrite(WALK_RED,HIGH);
    digitalWrite(SOUTH_RED,HIGH);
    digitalWrite(WEST_YELLOW,HIGH);
}

void Walk(void) {
    clearPins();
    digitalWrite(WALK_GREEN,HIGH);
    digitalWrite(SOUTH_RED,HIGH);
    digitalWrite(WEST_RED,HIGH);
}

void walkHurryOff(void) {
    clearPins();
    digitalWrite(WALK_GREEN,HIGH);
    digitalWrite(WALK_RED,HIGH);
    digitalWrite(SOUTH_RED,HIGH);
    digitalWrite(WEST_RED,HIGH);
}

void walkHurryOn(void) {
    clearPins();
    digitalWrite(WALK_RED,HIGH);
    digitalWrite(SOUTH_RED,HIGH);
    digitalWrite(WEST_RED,HIGH);
}
