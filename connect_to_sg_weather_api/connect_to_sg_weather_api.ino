int redLedPin = 2; //LED connected to digital pin 2
int yellowLedPin = 3; //LED connected to digital pin 3
int blueLedPin = 4; //LED connected to digital pin 4
int whiteLedPin = 5; //LED connected to digital pin 5
int greenLedPin = 6; //LED connected to digital pin 6
int val = 0;

void setup()
{
 int redLedPin = 2; 
int yellowLedPin = 3; 
int blueLedPin = 4; 
int whiteLedPin = 5; 
int greenLedPin = 6; 

 Serial.begin(9600);
}

void loop()
{
 val = Serial.read();
 if (val==27)
 { 
   digitalWrite(whiteLedPin, HIGH); //LED turns on
 }
  else if (val==28)
 { 
   digitalWrite(blueLedPin, HIGH); //LED turns on
 }
  else if (val==29)
 { 
   digitalWrite(yellowLedPin, HIGH); //LED turns on
 }
   else if (val==30)
 { 
   digitalWrite(greenLedPin, HIGH); //LED turns on
 }
   else if (val==31)
 { 
   digitalWrite(redLedPin, HIGH); //LED turns on
 }
   else if(val>31)
 { 
   digitalWrite(redLedPin, HIGH); //LED turns on
   digitalWrite(yellowLedPin, HIGH); //LED turns on
 }

 delay(100);
 
}
