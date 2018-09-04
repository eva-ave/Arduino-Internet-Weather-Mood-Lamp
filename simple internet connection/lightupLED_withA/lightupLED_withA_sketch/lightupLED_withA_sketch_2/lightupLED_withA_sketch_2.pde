import processing.serial.*;

// The serial port:
Serial myPort;

void setup() {
//List all available serial ports:
println(Serial.list());
myPort = new Serial(this, Serial.list()[1], 9600);
}

void draw() {
String[] text = new String[1];
text = loadStrings("https://eva-ave.github.io/lights/LED.txt"); //reads text on the url
myPort.write(text[0]); //sends "A" to the port
}
