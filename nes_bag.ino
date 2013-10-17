#include "SPI.h"
#include "Adafruit_WS2801.h"
#include <NESpad.h>

/*********************/
/* SETUP FOR NES PAD */
/*********************/
// 1: Strobe  :: TX on Flora
// 2: Clock   :: SDA on Flora
// 3: Data    :: SCL on Fl
NESpad nintendo = NESpad(1,2,3);
byte state = 0;

// Set buttons to false (non-pressed) by default
boolean a = false;
boolean b = false;
boolean u = false;
boolean d = false;
boolean l = false;
boolean r = false;
boolean s = false;
boolean e = false;

// Maintain order of keys pressed
int keysReleased[] = {
0,0,0,0, 0,0,0,0};

boolean isReleased = false;

/************************/
/* SETUP FOR LED PIXELS */
/************************/
uint8_t dataPin = 9;    // Yellow wire on Pixels (D9 on Flora)
uint8_t clockPin  = 10;   // Green wire on Pixels  (D10 on Flora)

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(24, dataPin, clockPin);

// Human-readable colors
uint32_t red = Color(200,0,0);
uint32_t off = Color(0,0,0);

// Default behavior is: all pixels on
uint8_t previous_pixel_instruction = 2;
uint8_t current_pixel_instruction  = 2;

void setup() {
  strip.begin();
  strip.show();  // Update LED contents, to start they are all 'off'
  allOn(red);
}

void loop() {
  state = nintendo.buttons();
  
  /************************/
  /* DETECT BUTTON EVENTS */
  /************************/
  // A Button
  if (state & NES_A){
    if(!a){      
      a = true; //Make sure the button is only pressed once 
      Serial.println("A pressed");
      current_pixel_instruction = 3; // party mode
    } else {
      a = false;
      keysReleased[0] = 1;
      isReleased = true;
    }
  }
  
  // B Button
  if (state & NES_B){
    if(!b){      
      b = true; //Make sure the button is only pressed once 
      Serial.println("B pressed");
    } else {
      b = false;
      keysReleased[1] = 1;
      isReleased = true;
    }
  }
  
  // UP Button
  if (state & NES_UP){
    if(!u){      
      u = true; //Make sure the button is only pressed once 
      Serial.println("UP pressed");
      current_pixel_instruction = 0; // Left turn
    } else {
      u = false;
      keysReleased[2] = 1;
      isReleased = true;
    }
  }
  
  // DOWN Button
  if (state & NES_DOWN){
    if(!d){      
      d = true; //Make sure the button is only pressed once 
      Serial.println("DOWN pressed");
      current_pixel_instruction = 1; // Right turn
    } else {
      d = false;
      keysReleased[3] = 1;
      isReleased = true;
    }
  }
  
  // LEFT Button
  if (state & NES_LEFT){
    if(!l){      
      l = true; //Make sure the button is only pressed once 
      Serial.println("LEFT pressed");
      current_pixel_instruction = 2; // All on
    } else {
      l = false;
      keysReleased[4] = 1;
      isReleased = true;
    }
  }

  // RIGHT Button
  if (state & NES_RIGHT){
    if(!r){      
      r = true; //Make sure the button is only pressed once 
      Serial.println("RIGHT pressed");
      current_pixel_instruction = 4; // All off
    } else {
      r = false;
      keysReleased[5] = 1;
      isReleased = true;
    }
  }
  
  // START Button
  if (state & NES_START){
    if(!s){      
      s = true; //Make sure the button is only pressed once 
      Serial.println("START pressed");
      current_pixel_instruction = 5; // Blink all
    } else {
      s = false;
      keysReleased[6] = 1;
      isReleased = true;
    }
  }

  // SELECT BUTTON
  if (state & NES_SELECT){
    if(!e){      
      e = true; //Make sure the button is only pressed once 
      Serial.println("SELECT pressed");
    } else {
      e = false;
      keysReleased[7] = 1;
      isReleased = true;
    }
  }

  // RESET EVENT DETECTION  
  if(isReleased){
    isReleased = false; //Reset the boolean
    for(int i=0; i < 8; i++){
      if(keysReleased[i] == 1){
        keysReleased[i] = 0; //Reset the button listener
      }
    }
  }

  if (current_pixel_instruction != previous_pixel_instruction) {
    previous_pixel_instruction = current_pixel_instruction;
  } 

  /*******************************/
  /* POSSIBLE PIXEL INSTRUCTIONS */
  /*******************************/
  /* 0: left turn                */
  /* 1: right turn               */
  /* 2: all on                   */
  /* 3: party mode               */
  /* 4: stealth mode             */
  /* 5: blink all                */
  /* 6: NOT CURRENTLY DEFINED    */
  /* 7: NOT CURRENTLY DEFINED    */
  /*******************************/  
  if (current_pixel_instruction == 0) {
      leftTurn(35);
    } else if (current_pixel_instruction == 1) {
      rightTurn(35);
    } else if (current_pixel_instruction == 2) {
      allOn(red);
    } else if (current_pixel_instruction == 3) {      
      party();
    } else if (current_pixel_instruction == 4) {    
      allOn(off);
    } else if (current_pixel_instruction == 5) {
      blinkAll(35);
    } else if (current_pixel_instruction == 6) {
      // NOT CURRENTLY DEFINED
    } else { // current_pixel_instruction == 7
      // NOT CURRENTLY DEFINED
    }
  
}

/***********************/
/* LED PIXEL FUNCTIONS */
/***********************/
void leftTurn(int wait_time) {
  allOn(red);
  
  int i;
  
  delay(wait_time);

  for (i=0; i<12; i++) {
    strip.setPixelColor(i, Color(200,0,0));
    strip.show();  
  }
  
  delay(wait_time);
  
  for (i=0; i<12; i++) {
    strip.setPixelColor(i, Color(0,0,0));
    strip.show();  
  }
}

void rightTurn(int wait_time) {
  allOn(red);
  
  int i;
 
  delay(wait_time);
  
  for (i=12; i<24; i++) {
    strip.setPixelColor(i, Color(200,0,0));
    strip.show();  
  }
  
  delay(wait_time);
  
  for (i=12; i<24; i++) {
    strip.setPixelColor(i, Color(0,0,0));
    strip.show();  
  }
}

void allOn(uint32_t color) {
  int i;
 
  for (i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show(); 
  }
}

void blinkAll(int wait_time) {
  allOn(red);
  delay(wait_time);
  allOn(off);
  delay(wait_time);
}

void party() {
  int rand_R = random(0, 256);
  int rand_G = random(0, 256);
  int rand_B = random(0, 256);
  int randPix = random(0, strip.numPixels());
  
  strip.setPixelColor(randPix, Color(rand_R, rand_G, rand_B));
  strip.show(); 
  delay(10);   
}

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b) {
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}
