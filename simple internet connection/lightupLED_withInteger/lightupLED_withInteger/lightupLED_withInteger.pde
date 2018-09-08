import processing.serial.*;

// The serial port:
Serial myPort;

void setup() 
{
  // List all available serial ports:
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 9600);
}

void draw() 
{
  myPort.write(65);
}
