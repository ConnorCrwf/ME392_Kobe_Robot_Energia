/*
  AnalogReadSerial
  Reads an analog input on pin A3, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A3, and the outside pins to ~3V and ground.
  
  Hardware Required:
  * MSP-EXP430G2 LaunchPad
  * 10-kilohm Potentiometer
  * hook-up wire

  This example code is in the public domain.
*/
// A4 = 33 = P5_1
// A15 = 2 =  P6_0
// A22 = P8_3 = 60

//int leftSensor = 60; 
//int rightSensor = 33; 

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600); // msp430g2231 must use 4800
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin A4:
  int leftSensorValue = analogRead(A4);
//   int rightSensorValue = analogRead(A16); 
//   int centerSensorValue = analogRead(A21); 
  // print out the value you read:
Serial.print("Left:  ");
    Serial.print(leftSensorValue);
    /*
     Serial.print("  Center:  ");
  Serial.print(centerSensorValue);
    Serial.print("  Right:  ");
  Serial.print(rightSensorValue);
  */
Serial.println();
  delay(100);        // delay in between reads for stability
}
