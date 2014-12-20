/*

  AutoCooker_Uno_V0.ino
  
  Copyright 2014 Sebastien Soudan 
  
  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at
  
  http://www.apache.org/licenses/LICENSE-2.0
  
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and 
  limitations under the License.
 
*/
// I2C/SPI includes
#include <Wire.h>

// Display includes
#ifndef ADAFRUIT_SSD1306__
  #define ADAFRUIT_SSD1306__
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
#endif

// Steps includes
#include "StepHolder.h"

// Relay includes
#include <TinkerKit.h>

// Temperature sensor includes
#include <max6675.h>

// I2C IO mux includes
#include <Adafruit_MCP23017.h>


/*
  Global objects
*/

// The MUX
Adafruit_MCP23017 mcp;

// The relay
#define RELAY_PIN 2
TKRelay relay(2);

// variables and constants to limit relay switching frequency
#define MIN_DURATION 10*1000 // in ms
unsigned long lastChange;
boolean lastState;

// The thermocouple
#define T_CLK 7
#define T_CS 6
#define T_D0 5
MAX6675 thermocouple(T_CLK, T_CS, T_D0);

// variables to limit temperature read frequency
#define TEMPERATURE_READ_COUNT 2
int i = 0;
float temperatureC;

// The display
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// That's where we store/manage the steps
StepHolder steps;

/*
  Setup function (only called once)
*/
void setup() {

  // initialize serial port
  Serial.begin(115200);
  Serial.println("AutoCooker V0");

  // switch the relay off
  relay.off();

  // and remember that
  lastChange = millis();
  lastState = false;
  
  // initialize the IO mux
  // use default address 0
  mcp.begin();      

  mcp.pinMode(7, INPUT);
  mcp.pullUp(7, HIGH);  // turn on a 100K pullup internally

  mcp.pinMode(6, INPUT);
  mcp.pullUp(6, HIGH);  // turn on a 100K pullup internally

  mcp.pinMode(5, INPUT);
  mcp.pullUp(5, HIGH);  // turn on a 100K pullup internally

  mcp.pinMode(4, INPUT);
  mcp.pullUp(4, HIGH);  // turn on a 100K pullup internally

  // initialize with the I2C addr 0x3D
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  // clears the screen and buffer
  display.clearDisplay();

  Serial.println("Done with setup()");
}

/*
  Main loop (called in a loop after setup() has been called)
*/
void loop() {

  // get the state of the buttons from the I2C mux
  boolean button0 = !mcp.digitalRead(7);
  boolean button1 = !mcp.digitalRead(6);
  boolean button2 = !mcp.digitalRead(5);
  boolean button3 = !mcp.digitalRead(4);

  // do the corresponding action
  if (button0) {
    steps.prev();
  }
  if (button1) {
    steps.inc();
  }
  if (button2) {
    steps.dec();
  }
  if (button3) {
    steps.next();
  }

  // read the temperature and update the current step
  // every (TEMPERATURE_READ_COUNT+1) rounds (each round being 100 ms
  // + the duration of I2C operations)
  if (i++ == TEMPERATURE_READ_COUNT) {
    temperatureC = thermocouple.readCelsius();
    if (!isnan(temperatureC)) {
      Serial.print("C = "); 
      Serial.println(temperatureC);
      steps.setCurrentTemperature(temperatureC);
    }
    i = 0;
  }

  // update the internale state of the step holder with regards to the current time
  unsigned long now = millis();
  steps.updateElapsedTime(now);

  // state has been updated, we can now refresh the OLED display with current information
  steps.display(display);

  // we don't want to change the state of the relay too frequently
  if (now - lastChange > MIN_DURATION) 
  {

    // if current state of the relay is not the expected one, change it.
    if (steps.isOn() && !lastState) 
    {
      relay.on();      
      Serial.println("Heating!");
      lastChange = now;
      lastState = true;
    } 
    else if (!steps.isOn() && lastState)
    {
      relay.off();
      Serial.println("NOT heating!");
      lastChange = now;
      lastState = false;
    }     
  }  
  
  // sleep a bit
  delay(100);  
}
