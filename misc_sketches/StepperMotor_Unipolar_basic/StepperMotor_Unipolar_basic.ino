///This works for Unipolar Stepper motors interfaced with the L297 H-Bridge Motor Driver
//Refer to picture for pinout and schematic
//Unipolar doesn't use the black wire. It uses the brown, green, red, and white.      
 
 /* Unipolar STEPPER MOTOR TEMPLATE CODE
 -----------------------------------------------------*/
#define  COIL1a       34 //P2.3 brown,   corresponds to IN1
#define  COIL1b       38  //38 P2.4 gray, corresponds to IN3
#define  COIL2a       19 //P2.5 purple, corresponds to IN2
#define  COIL2b       32 //P3.5 orange, corresponds to IN4

//in the past pins used were 8,9,10,11 going down 


//Refer to picture of stepper motor hooked up to L298 to know where diff color wires are connected

//if definitions are above are believed correct, then the following is true about the wires coming out of stepper motor
//then Red and Brown are on the same coil
//and Green and white are on the same coil

//so the following could be true
//so CoilA pos is Coil1A red
//CoilA neg is Coil1B brown
//so Coi2A pos is Coil2A green
//Coil2 neg is Coil2B white

int timeDelay = 25;

void setup()
{

}

void loop(){

// Turn on and off each coil in sequence   
// Turn on and off each coil in sequence  
// can maybe think of it as 
//Coil A positive
//then Coil A negative
//then Coil B negative 
//then Coil B positive 
//I don't think In1-4 are directly linked to Coil1a,1b,2a,2b, but look above for explanation
digitalWrite(COIL1a, HIGH);  //IN1
delay(timeDelay); 
digitalWrite(COIL1a, LOW);   //IN1
digitalWrite(COIL1b, HIGH);  //IN3
delay(timeDelay); 
digitalWrite(COIL1b, LOW);  //IN3
digitalWrite(COIL2a, HIGH); //IN2
delay(timeDelay); 
digitalWrite(COIL2a, LOW);  //IN2
digitalWrite(COIL2b, HIGH); //IN4
delay(timeDelay); 
digitalWrite(COIL2b, LOW);  //IN4

}
