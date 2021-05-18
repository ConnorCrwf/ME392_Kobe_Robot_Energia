/*
  DigitalReadSerial with on-board Pushbutton
  Reads a digital input on pin 5, prints the result to the serial monitor 
 
  Harware Required:
  * MSP-EXP430G2 LaunchPad
  
  This example code is in the public domain.
 */

// digital pin 5 has a pushbutton attached to it. Give it a name:
int leftSensor = 2; // A4 = 33 = P5_1, A2 = 2, P6_0, A22 - P8_3 - 60
int rightSensor = 33; // A4 = 33 = P5_1, A2 = 2, P6_0, A22 - P8_3 - 60


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600); // msp430g2231 must use 4800
  // make the on-board pushbutton's pin an input pullup:
  pinMode(leftSensor,INPUT);
//  digitalWrite(leftSensor,LOW);
//  pinMode(leftSensor, INPUT_PULLUP);
//  pinMode(rightSensor, INPUT_PULLUP);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  int sensorStateLeft = digitalRead(leftSensor); // THEY ARE ACTIVE LOW SO 1 MEANS OFF
//    int sensorStateRight = digitalRead(rightSensor);
    
  // print out the state of the button:
   Serial.print("Left:  ");
    Serial.print(sensorStateLeft);
//    Serial.print("  Right:  ");
//  Serial.print(sensorStateRight);
  Serial.println();
  delay(100);        // delay in between reads for stability
}
