int ledPin = 13; //LED connected to digital pin 13
String val = "";

void setup()
{
 pinMode(ledPin, OUTPUT);//digital pin set as output
 Serial.begin(9600);
}

void loop()
{
   val = Serial.read();
   if (val == "65")
   {  
     digitalWrite(ledPin, HIGH); //LED turns on
   }


}
