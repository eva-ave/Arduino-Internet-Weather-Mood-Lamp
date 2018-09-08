import processing.serial.*;

// The serial port:
Serial myPort;

void setup() {
//List all available serial ports:
println(Serial.list());
myPort = new Serial(this, Serial.list()[0], 9600);
}

void draw() {
String[] text = new String[1];
text = loadStrings("https://eva-ave.github.io/lights/LED.txt"); //reads text on the url
int textToInt = int(text[0]);
myPort.write(textToInt); //sends "A" to the port
}
