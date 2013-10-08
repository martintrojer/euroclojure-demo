/*
Xively Euroclojure Demo

This code receives the serial message from the imp, parses the raw accell value
and lights up the corresponding number of LEDs.

Setup:
-LED Strip connect on pin 6
-imp connect on pins 8,9
-Common ground for LED and Arduino power

License:
BSD-3 Clause

Author:
Calum Barnes, Xively Systems Enginer, September 2013
Martin Trojer, Xively Developer, October 2013

*/
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(171, PIN, NEO_GRB + NEO_KHZ800);

SoftwareSerial impSerial(8, 9); // RX on 8, TX on 9

// -----------------------------------------
// Arduino entry points

void setup() {
  Serial.begin(19200);

  impSerial.begin(19200);

  // Setup and clear the LEDs
  strip.begin();
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);//off
  strip.show();

  Serial.println("==STARTING==");
}

char inData[100];
char inChar=-1;
byte index = 0;
String input = "";
boolean newdata = false;

void loop() {
  while (impSerial.available() > 0) {
    inChar = impSerial.read();             // Read a character
    inData[index] = inChar;                // Store it
    index++;                               // Increment where to write next
    inData[index] = '\0';                  // Null terminate the string
    Serial.print(inChar);
    newdata = true;
  }

  index = 0;

  // read and parse the data

  if(newdata) {
    Serial.println("new data");
    if(inData[0] == 'X') {
      int i = 1;
      while(inData[i] != 'Q') {
       input.concat(inData[i]);
       i++;
      }
    }

   // LEDs

   Serial.println(input);
   setLEDs(parseInt(input));
   newdata = false;
   input = "";
  }
}

// -----------------------------------------

int parseInt(String s) {
  char chars[255];
  s.toCharArray(chars, sizeof(chars));
  return atoi(chars);
}

uint32_t getColor(int v) {
  if (v < 40)
    return strip.Color(255,0,0);
  else if (v < 70)
    return strip.Color(0,255,0);
  else if (v < 100)
    return strip.Color(0,0,255);
  else
    return strip.Color(255,255,255);
}

void setLEDs(int tofill) {
  Serial.print("Setting LEDs; "); Serial.println(tofill);
  for(int i=0; i<107; i++) {
    if (i <= tofill)
      strip.setPixelColor(i, getColor(i));
    else
      strip.setPixelColor(i, 0);
  }
  if (tofill>=107)
    for (int i=107; i<strip.numPixels(); i++)
      strip.setPixelColor(i, strip.Color(0,133,207));  // Xively Blue
  else
    for (int i=107; i<strip.numPixels(); i++)
      strip.setPixelColor(i, 0);

  strip.show();
}
