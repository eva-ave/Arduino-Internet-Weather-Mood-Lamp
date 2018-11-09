# Simple Arduino Weather Mood Lamp

This simple project uses simple RGB LEDs to output a certain colour based on the weather. The weather information will be extracted from the weather API without the use of an ethernet/internet shield. (Awesome if you're a broke student...like me XD)

What we will do here is open a webpage with a Processing code and retrieve the relevant data from the webpage. Then, this data will be sent to the port and our arduino will read the data sent at the port and respond accordingly. 

## Getting Started

Things you'll need:
1. Arduino UNO Board
2. Neopixels (RGB LEDs do the trick as well!)
3. some 270 ohm resistors
4. wires
5. [Processing software](https://processing.org/download)
6. [Weather webpage](https://api.data.gov.sg/v1/environment/air-temperature) (I'm living in Singapore, so naturally, I'll be using the SG weather API. Feel free to accomodate to your location's weather API!)

## 1. Dig up a webpage with weather information

What you will need is a webpage that contains relevant weather information. Lucky for me, I have access to [the free weather API](https://api.data.gov.sg/v1/environment/air-temperature) in my country(I am living in Singapore) courtesy of the National Environment Agency(NEA). It has temperatures recorded at the several weather stations around Singapore and is accurate by the minute.

![alt text](https://github.com/eva-ave/Arduino-Internet-Weather-Mood-Lamp/blob/master/images_for_read.me/api_screenshot.PNG "Singapore is a small and awesome place, really :)")

If you decide to use the same webpage as I do, do note though, that the data provided can be quite unstable. At some minutes, the data from a specific weather station may not be available. Keep refreshing the page every minute and you'll see what I mean. In that case, you might want to conside an edge case in your code.

## 2. Write a Processing code to parse the information on the webpage

This is where we are trying to get the arduino to retrieve the weather information. This is also where the code is dependent on which webpage you are using. I leave this exploration part to you but here's my Processing code I used to parse the page mentioned above:

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

If you are using a different weather webpage, our codes will be different. But I would like to point out 2 important lines in the code: 

```
JSONObject weatherDataPage = loadJSONObject("https://api.data.gov.sg/v1/environment/air-temperature");
```

This line is the real deal; the arduino was never designed for it to be hooked up to the internet. This line allows us to open up a webpage with the arduino. The rest of the code is just trying to parse the relevant data at the specific station. If you have not used Processing before, fret not. It is really quite simple. I would suggest you look up the [reference for Processing](https://processing.org/reference) to get started.

```
myPort.write(weatherValue); 
```

This is the next real deal. myPort.write() is the built-in function that sends the temperature value to the port. 

### 2.1 Going into the nitty-gritty of my Processing code

```java
void setup() 
{
  //List all available serial ports:
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 9600);
}
```

In the setup() function, what I'm doing is listing the ports that are in use in the form of an array. Then, depending of which port you are using, assign the specific index of the array to myPort. For example, if COM1 and COM4 gets printed respectively, and your arduino is connected to COM1, that means COM1 is the first index of the array, and thus the '0' index above. (I won't go into the specifics of how to know which port your arduino is connected to... just google it.)

```java
  JSONObject weatherDataPage = loadJSONObject("https://api.data.gov.sg/v1/environment/air-temperature"); //reads text on the url;
  JSONArray itemArr = weatherDataPage.getJSONArray("items");
  JSONObject innerItemObj = itemArr.getJSONObject(0);
  JSONArray readingsArr = innerItemObj.getJSONArray("readings");
```

As mentioned, the first line opens up the webpage and I assign this webpage to the JSONObject variable weatherDataPage. 
In the second line, I access the "items" array and assign it to JSONArray variable itemArr. In the third line, I'm accessing the first element of the "items" array (which essentially only has one element if you look closely). The first element has 2 objects, "timestamp" and "readings". In the next line, I then access "readings" object and assign it to readingsArr variable.

```java
  for(int i = 0; i < arrSize ; i++)
  {
    JSONObject stationInfo = readingsArr.getJSONObject(i);
    String stationIdCheck = stationInfo.getString("station_id");
  
    if(stationIdCheck.equals("S60"))
    {
      weatherValue = stationInfo.getFloat("value");
      break;
    }
  
  }
  ```

The above for-loop runs through the objects in the "readings" array and the if-loop finds if a specific 'stationId' is available.

## 3. Arduino code! 

This is the main character in getting the arduino to respond to the data. 

```
#include <Adafruit_NeoPixel.h>
 
#define PIN 6
#define N_LEDS 8
#define BRIGHTNESS 50

int val = 0;
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
 
void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show();
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

The three values in the bracket are the RGB values, in the sequence of red, green, blue. For example, the combination (0, 0, 255) gives the most distinct shade of blue.

(include neopixel gif)

## 4. Hook up the circuit

This is the simplest part. I am using a neopixel, which really is just a series of RGB LEDS on a strip. You could of course use a simple RGB LED too. But I think neopixels are cooler XD. Do note that if you are using a neopixel and it has four pads on it, you'll only need to solder wires to three of them which is the ground, V-out and the V-in. 

![alt text](/images_for_read.me/neopixel_circuit_schematic.PNG?raw=true "Hooking up my neopixels to my arduino board")

## 5. Ta-da!

Take note here that the sequence you run the two codes here is important. First, the arduino code, then the processing code.

## Moving on from here

I always believe a project can always be made better. My next goal is to provide the arduino uno board constant internet access even when it is not connected to the laptop. Not too sure how yet, but I'll start hacking a way soon! Do watch this space if you're interested! :)

### Many Thanks to:

1. An inspiration from an online article that I cannot find now :(
2. The Makers' community at my internship company Autodesk for providing the mini neopixel XD

Cheers! XD
