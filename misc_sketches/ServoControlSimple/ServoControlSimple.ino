//could i just attach these to power and ground since the pwm is what varies?
//Connect power and ground lines to 5V and Ground
const int PWMspeed = 17;

 // These are pins on the Launchpad
 /*
#define  PWMspeed 17    //white P5.7
*/


void setup()
{
  // put your setup code here, to run once:
    Serial.begin(9600);
 pinMode(PWMspeed, OUTPUT);
}

void loop()
{
  analogWrite(PWMspeed, 90);
  // analogWrite(PWMoutn,0);

  delay(10);

  
}
