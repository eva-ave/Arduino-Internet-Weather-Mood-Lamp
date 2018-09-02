int ledPin = 13; //LED connected to digital pin 13
int val = 0;

void setup()
{
 pinMode(ledPin, OUTPUT);//digital pin set as output
 Serial.begin(9600);
}

void loop()

{
 val = Serial.read();
 if (val == 'A'){
   
   digitalWrite(ledPin, HIGH); //LED turns on
 }

}
