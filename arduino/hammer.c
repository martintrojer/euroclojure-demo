/*
HAMMERTIME v2

Arduino firmware for Hammertime game Xivley demo. This demonstration
is a recreationg of the clasic strong man carnival game reproduced using
a phones accelerometer, harvested with a mobile app, Xively, electric imp for
WiFi connectivity, Arduino, and Adafruit NeoPixel LED strips.

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

//wait
int lwait = 50;

SoftwareSerial impSerial(8, 9); // RX on 8, TX on 9

char inData[100]; 
char inChar=-1; 
byte index = 0; 
String input = "";
boolean newdata = false;



void setup(){
  Serial.begin(19200);
  
  impSerial.begin(19200);
  
   // Start up the LED strip
  strip.begin();
  
  //clear strip for startup
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);//off  
  strip.show();
  
  Serial.println("==STARTING==");
}//end setup

void loop(){
  while (impSerial.available() > 0){

            inChar = impSerial.read(); // Read a character
            inData[index] = inChar; // Store it
            index++; // Increment where to write next
            inData[index] = '\0'; // Null terminate the string  
            Serial.print(inChar);
            newdata = true;
    }
    
    index = 0;
    
  
    if(newdata){ 
      Serial.println("new data");
       if(inData[0] == 'X'){
           int i = 1;
          while(inData[i] != 'Q'){
             input.concat(inData[i]);
             i++;
          } 
       }
       Serial.println(input);
       int ledfill = parseNum(input);
       drawLED(ledfill);
       
       newdata = false;
       input = "";
    }
    
    
///////try to rainbow///////////////////////////////////////////can remove below this line
    //int j = 0;
    uint16_t i, j;
    //rainbow cycle
//    for(i=0; i< strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
//    }

    //rainbow
//    for(i=0; i<strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel((i+j) & 255));
//    }
//    strip.show();
//    if(j > 256){  //255?
//      j=0;
//    }else j++;
}//end loop


/////////////////////////////FUNCTIONS///////////////////////////////////
//custom function to parse values from accell to number of leds
int parseNum(String raw){
  int returnfill;
  
  char chars[10];
  raw.toCharArray(chars, sizeof(chars));
  float floatval = atof(chars);
  
  Serial.println(floatval);
  int theval = floatval * 100;
  
  
   //map magic
    //returnfill = map(floatval, 0, 80, 0, 64);
    
   int ledv[] = {0,40,60,65,70,75,80,85,90,95,100,107};
  // in[] holds the measured analogRead() values for defined distances
  int accelv[]  = {0,2000,4000,5000,7000,7500,7600,7700,7900,7950,8000,8030};
  //float val = 75;
  returnfill = multiMap(theval, accelv, ledv, 12);
  
  Serial.print("out=");
  Serial.println(returnfill);
  
  
  return returnfill;
}

//funciton that actually lights up LEDs. Color tiering occurs here
void drawLED(int tofill){
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
    if(tofill>40){
        colorWipe(strip.Color(  255, 0,   0), lwait, 0, 40);  // red
        if(tofill>70){
            colorWipe(strip.Color(255, 255,   0), lwait, 40, 70); // Yellow
            if(tofill>=107){
                colorWipe(strip.Color(0,   255,   0), lwait, 70, 107);  // green
                //colorWipe(strip.Color(0,   133,   207), lwait, 102, 107);  // blue
                colorWipe(strip.Color(0,133,207), 0, 108, 171); // Xivley Blue
                //win
                win();
            }else colorWipe(strip.Color(0,   255,   0), lwait, 70, tofill);  // green
        }else colorWipe(strip.Color(255, 255,   0), lwait, 40, tofill); // Yellow
    }else colorWipe(strip.Color(  255, 0,   0), lwait, 0, tofill);  // red
    
}

//function run when leds reach the win value
void win(){
   Serial.println("win");
   //clear for win sequence
   for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
   //new light box
   for(int i=0; i<6; i++){
     colorWipe(strip.Color(0,133,207), 0, 0, 171); // Xivley Blue
     //delay(100);
     for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
     //delay(100);
   }
}

////////////* Helper functions *///////////////
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

// custom wipe to Fill the dots progressively along the strip.
void colorWipe(uint32_t c, uint8_t wait, int fills, int fille) {
  //for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);   //clear all leds
  //int i;

  for (int i=fills; i < strip.numPixels(); i++) {
    if(i < fille){
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
    }
  }
  
}

//function to array map the accel values to number of leds
int multiMap(int val, int* _in, int* _out, uint8_t size)
{
  // take care the value is within range
  // val = constrain(val, _in[0], _in[size-1]);
  if (val <= _in[0]) return _out[0];
  if (val >= _in[size-1]) return _out[size-1];

  // search right interval
  uint8_t pos = 1;  // _in[0] allready tested
  while(val > _in[pos]) pos++;

  // this will handle all exact "points" in the _in array
  if (val == _in[pos]) return _out[pos];

  // interpolate in the right segment for the rest
  return map(val, _in[pos-1], _in[pos], _out[pos-1], _out[pos]);
}
