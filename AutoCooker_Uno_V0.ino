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
#include <Wire.h>

#ifndef ADAFRUIT_SSD1306__
  #define ADAFRUIT_SSD1306__
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
#endif

#include "StepHolder.h"

#include <TinkerKit.h>

#include <math.h>
#include <max6675.h>

#include <Adafruit_MCP23017.h>

Adafruit_MCP23017 mcp;

#define RELAY_PIN 2
TKRelay relay(2);

#define T_CLK 7
#define T_CS 6
#define T_D0 5
MAX6675 thermocouple(T_CLK, T_CS, T_D0);

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define MIN_DURATION 10*1000 // in ms

unsigned long lastChange;
boolean lastState;

int i = 0;
float temperatureC;

StepHolder steps;

void setup() {
  Serial.begin(115200);
  Serial.println("AutoCooker V0");

  lastChange = millis();
  lastState = false;
  relay.off();
  
  mcp.begin();      // use default address 0

  mcp.pinMode(7, INPUT);
  mcp.pullUp(7, HIGH);  // turn on a 100K pullup internally

  mcp.pinMode(6, INPUT);
  mcp.pullUp(6, HIGH);  // turn on a 100K pullup internally

  mcp.pinMode(5, INPUT);
  mcp.pullUp(5, HIGH);  // turn on a 100K pullup internally

  mcp.pinMode(4, INPUT);
  mcp.pullUp(4, HIGH);  // turn on a 100K pullup internally

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // display.display(); // show splashscreen
  // delay(2000);
  display.clearDisplay();   // clears the screen and buffer

  Serial.println("Done with setup()");
}

/*
  Main loop
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
  // every 3 rounds (~300 ms)
  if (i++ == 2) {
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
