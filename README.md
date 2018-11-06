# Simple Arduino Weather Mood Lamp

This simple project uses simple RGB LEDs to output a certain colour based on the weather. The weather information will be extracted from the weather API without the use of an ethernet/internet shield. (Awesome if you're a broke student...like me XD)

What we will be doing here is open a webpage with a Processing code and retrieve the relevant data from the webpage. Then, we will send it to the port and our arduino will read the data sent at the port and respond accordingly. 

## Getting Started

Things you'll need:
1. Arduino UNO Board
2. Neopixels (RGB LEDs do the trick as well!)
3. some 270 ohm resistors
4. wires
5. [Processing software](https://processing.org/download)
6. [Weather webpage](https://api.data.gov.sg/v1/environment/air-temperature) (I'm living in Singapore, so naturally, I'll be using the SG weather API. Feel free to accomodate to your location's weather API!)

## 1. Dig up a webpage with weather information

What you need is a webpage that contains relevant weather information. Lucky for me, I'm living in Singapore and with the courtesy of the National Environment Agency(NEA) I am using their free weather API. [Their API](https://api.data.gov.sg/v1/environment/air-temperature) has temperatures recorded at the several weather stations in Singapore that is accurate by the minute.

![alt text](images_for_read.me/api_screenshot.png "Singapore is a small place, really :)")

## 2. Write a Processing code to parse the information on the webpage

This is where we are trying to get the arduino to retrieve the weather information. Here's where things are dependent on which webpage you are using to parse the relevant information. My Proceesing code that I used to parse the page mentioned above is the following:

```
import processing.serial.*;

Serial myPort;

void setup() 
{
  //List all available serial ports:
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 9600);
}

void draw() 
{
  JSONObject weatherDataPage = loadJSONObject("https://api.data.gov.sg/v1/environment/air-temperature"); //reads text on the url;
  JSONArray itemArr = weatherDataPage.getJSONArray("items");
  JSONObject innerItemObj = itemArr.getJSONObject(0);
  JSONArray readingsArr = innerItemObj.getJSONArray("readings");
  int arrSize = readingsArr.size();
  float weatherValue = 0.0;
  int decimalInt = 10;
  
  // the following for loop is to determine the size of the array
  for( int i = 0; i < arrSize ; i++)
  {
    JSONObject stationInfo = readingsArr.getJSONObject(i);
    String stationIdCheck = stationInfo.getString("station_id");
  
    if( stationIdCheck.equals("S60"))
    {
      weatherValue = stationInfo.getFloat("value");
      break;
    }
  
  }
  
  println(weatherValue); //optional; for debugging purposes
  
  myPort.write(weatherValue); 

}
```

But I would like to point out this most important line: 

```
JSONObject weatherDataPage = loadJSONObject("https://api.data.gov.sg/v1/environment/air-temperature");
```

This line is the real deal; the arduino was never designed for it to be hooked up to the internet; this line allows us to open up a webpage with the arduino. The rest of the code is just trying to parse the relevant data at the specific station. If you haven't used Processing before, fret not. It's really quite simple. I would suggest you look up the [reference for Processing](https://processing.org/reference) to get started.

### 2.1 Going into the nitty-gritty of my Processing code

```
void setup() 
{
  //List all available serial ports:
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 9600);
}
```
In the setup() function, what I'm doing is listing the ports that are in use. This prints out an array of the ports in use. Then, depending of which port I am using, I assign the specific index of the array. For example, if COM1 and COM4 gets printed, and your arduino is connected to COM1, that is the first index of the array, and thus the '0' index above. (I won't go into the specifics of how to know which port your arduino is connected to... just google it.)

```
myPort.write(weatherValue); 
```

myPort.write() is the built-in function that sends the temperature value to the port.

## 3. Arduino code! 

This is the main character in getting the arduino to respond to the data. 

```
// Simple NeoPixel test.  Lights just a few pixels at a time so a
// 1m strip can safely be powered from Arduino 5V pin.  Arduino
// may nonetheless hiccup when LEDs are first connected and not
// accept code.  So upload code first, unplug USB, connect pixels
// to GND FIRST, then +5V and digital pin 6, then re-plug USB.
// A working strip will show a few pixels moving down the line,
// cycling between red, green and blue.  If you get no response,
// might be connected to wrong end of strip (the end wires, if
// any, are no indication -- look instead for the data direction
// arrows printed on the strip).
 
#include <Adafruit_NeoPixel.h>
 
#define PIN 6
#define N_LEDS 8
#define BRIGHTNESS 50

int val = 0;
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
 
void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  
  Serial.begin(9600);
}
 
void loop() {

  val = Serial.read();
  if(val == 0)
  { 
    chase( strip.Color(255, 0, 0) );  //red; appears when data cannot be retrieved
  }
    else if(val < 25)
  { 
    chase( strip.Color(0, 0, 255) );  //dark blue
  }
    else if(25 < val <= 26)
  { 
    chase( strip.Color(35, 0, 255) );  //dark purple
  }
  else if(26 < val <= 27)
  { 
    chase( strip.Color(0, 255, 255) );  //light turquoise
  }
  else if(27 < val <= 28)
  { 
    chase( strip.Color(50, 50, 255) );  //light blue
  }
  else if(28 < val <= 29)
  { 
    chase( strip.Color(255, 255, 0) );  //yellow
  }
  else if(29 < val <= 30)
  { 
    chase( strip.Color(255, 35, 0) );  //orange
  }
  else if(val > 30)
  { 
    chase( strip.Color(255, 0, 255) );  //bright pink
  }

  delay(50);
}
 
static void chase(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels()+8; i++) {
      strip.setPixelColor(i, c); // Draw new pixel
      strip.setPixelColor(i-8, 0); // Erase pixel a few steps back
      strip.show();
      delay(350);
  }
}
```

### 3.1 Diving deeper into the code
Basically, I chose different colours for different temperature ranges. For example:

```
chase( strip.Color(0, 0, 255) );  //dark blue
```
"chase" is a function that comes with the neopixel library. With "chase", the lights will run down the neopixel strip as if was chasing something. The three values in the bracket are the RGB values, in the sequence of red, green, blue. For example, the combination (0, 0, 255) gives the most distinct shade of blue.

(include neopixel gif)

## 4. Hook up the circuit

This is the simplest part. I am using a neopixel, which really is just a series of RGB LEDS on a strip. You could of course use a simple RGB LED too. But I think neopixels are cooler XD. Do note that if you are using a neopixel and it has four pads on it, you'll only need to solder wires to three of them which is the ground, V-out and the V-in. 

![alt text](images_for_read.me/neopixel_circuit_schematic.png "Hooking up my neopixels to my arduino board")

## 5. Ta-da!
Take note here that the sequence you run the two codes here is important. First, the arduino code, then the processing code.

## Moving on from here
Nope my project is still ongoing. There are still areas of improvement. For example, to provide the arduino uno board constant internet access when it is not connected to the laptop. Not too sure how yet, but I'll start hacking a way soon! Do watch this space if you're interested! :)
