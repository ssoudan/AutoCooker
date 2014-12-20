/*
 Step.cpp
 
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
#include "Step.h"


void        Step::
display(Adafruit_SSD1306 & display, boolean isRunning)
{
    display.clearDisplay();
    display.setTextSize(1);

    if (isRunning) 
    {
        display.setCursor(128 - 6 * 6, 0);
        display.setTextColor(BLACK, WHITE);
        display.print("ACTIVE");

        display.setCursor(2, 54);
        display.setTextColor(BLACK, WHITE);
        if (isOn()) 
        {
            display.print("/!\\ HEATING /!\\");
        } 
        else 
        {
            display.print("COOLING");
        }
    }
    display.drawRect(0, 10, display.width(), display.height() - 10, WHITE);

    display.setTextColor(WHITE, BLACK);

    display.setCursor(0, 0);
    display.print("Step #");
    display.print(step_number);

    display.setCursor(2, 12);
    display.print("Current: ");
    display.print(current_temperature);
    display.print(" C");

    display.setCursor(2, 22);
    display.print("Target: ");
    if (selected == TEMPERATURE_SELECTOR) 
    {
        display.setTextColor(BLACK, WHITE);
    }
    display.print(target_temperature);
    display.print(" C");
    if (selected == TEMPERATURE_SELECTOR) 
    {
        display.setTextColor(WHITE, BLACK);
    }
    display.setCursor(2, 32);
    display.print("Duration: ");
    if (selected == DURATION_SELECTOR) 
    {
        display.setTextColor(BLACK, WHITE);
    }
    display.print(target_duration);
    display.print(" min");
    if (selected == DURATION_SELECTOR) 
    {
        display.setTextColor(WHITE, BLACK);
    }
    if (isRunning) 
    {
        display.setCursor(2, 42);
        display.print("Remaining: ");
        display.print(target_duration - elapsed_duration);
        display.print(" min");
    }
    display.display();
}

void        Step::
updateElapsedTime(unsigned long millis)
{

    if (!this->done) {
        if (this->start_time == 0) {
            this->start_time = millis;
        }
        this->elapsed_duration = (millis - this->start_time) / 1000 / 60;

        if (this->elapsed_duration >= this->target_duration) 
        {
            this->done = true;
        }
    }
}

void        Step::
inc()
{
    switch (selected) 
    {
    case DURATION_SELECTOR:
        incDuration();
        break;
    case TEMPERATURE_SELECTOR:
        incTemperature();
        break;
    default:
        //NOTHING
            ;
    }
};

void        Step::
dec()
{
    switch (selected) 
    {
    case DURATION_SELECTOR:
        decDuration();
        break;
    case TEMPERATURE_SELECTOR:
        decTemperature();
        break;
    default:
        //NOTHING
            ;
    }
};
