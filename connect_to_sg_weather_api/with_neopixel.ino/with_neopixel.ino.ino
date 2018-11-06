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
    chase( strip.Color(0, 0, 255) );  //dark blue
  }
  else if(val == 1)
  { 
    chase( strip.Color(0, 255, 255) );  //light turquoise
  }
  else if(val == 2)
  { 
    chase( strip.Color(35, 0, 255) );  //dark purple
  }
  else if(val == 3)
  { 
    chase( strip.Color(50, 50, 255) );  //light blue
  }
  else if(val == 4)
  { 
    chase( strip.Color(0, 255, 0) );  //green
  }
  else if(val == 5)
  { 
    chase( strip.Color(255, 255, 0) );  //yellow
  }
  else if(val == 6)
  { 
    chase( strip.Color(255, 35, 0) );  //orange
  }
  else if(val == 7)
  { 
    chase( strip.Color(255, 0, 255) );  //bright pink
  }
  else if(val == 8)
  { 
    chase( strip.Color(255, 0, 0) );  //red
  }
  else if(val == 9)
  { 
    chase( strip.Color(255, 255, 255) );  //white
  }
  else if(val == 10)
  { 
    chase( strip.Color(25, 0, 0) );  //dim red
  }

  delay(50);
}
 
static void chase(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels()+8; i++) {
      strip.setPixelColor( i, c); // Draw new pixel
      strip.setPixelColor( i-8, 0); // Erase pixel a few steps back
      strip.show();
      delay(350);
  }
}
