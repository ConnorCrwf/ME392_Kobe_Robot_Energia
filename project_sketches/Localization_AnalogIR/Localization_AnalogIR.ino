/*
  AnalogReadSerial
  Reads an analog input on pin A3, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A3, and the outside pins to ~3V and ground.
  Signal will go down to 0 once the transmitted IR signal is detected by the Receiver
  Need to cover up with aluminum foil and make sure there is no gap where an IR signal could creep in. They can get in the tiniest of spots
  
  Hardware Required:
  * MSP-EXP430G2 LaunchPad
  * 10-kilohm Potentiometer
  * hook-up wire

  This example code is in the public domain.
*/

//Hook up Power from the sensor board to 3.3 Volts
//Press the S2 switch to go between different beacons, use the Beacon Flash code
int centerSensorPin = 60; //P8.3
int rightSensorPin = 33; //P5.1

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600); // msp430g2231 must use 4800
}

// the loop routine runs over and over again forever:
void loop() {
  int centerSensorValue = analogRead(centerSensorPin);
//   int rightSensorValue = analogRead(rightSensorPin); 
//   int centerSensorValue = analogRead(A21); 
  // print out the value you read:
    Serial.print("Center:  ");
    Serial.print(centerSensorValue);
//    Serial.print("  Right:  ");
//    Serial.print(rightSensorValue);
    //     Serial.print("  Center:  ");
//  Serial.print(centerSensorValue);
 
  Serial.println();
  delay(100);        // delay in between reads for stability
}
