/*
 Step.h
 
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
#ifndef __STEP_H__
#define __STEP_H__

#ifndef ADAFRUIT_SSD1306__
#define ADAFRUIT_SSD1306__
    #include <Adafruit_GFX.h>
    #include <Adafruit_SSD1306.h>
#endif

#define TEMPERATURE_SELECTOR 0
#define DURATION_SELECTOR 1
#define MAX_SELECTOR 1

#define DEFAULT_DURATION 60
#define DEFAULT_TARGET_TEMPERATURE 65

class Step 
{
    
private:
    uint8_t step_number;
    
    uint8_t target_duration;

    uint8_t elapsed_duration;

    uint16_t target_temperature;

    float current_temperature;

    unsigned long start_time;

    boolean done;

    uint8_t selected;

    void incTemperature() { if (!isDone() && target_temperature < 0xffff) target_temperature++; };
    void decTemperature() { if (!isDone() && target_temperature > 0x0000) target_temperature--; };

    void incDuration() { if (!isDone() && target_duration < 0xffff) target_duration++; };
    void decDuration() { if (!isDone() && target_duration > 0x0000) target_duration--; };

public:
    Step() 
    {
        this->step_number = 0;
        this->target_duration = DEFAULT_DURATION;
        this->elapsed_duration = 0;
        this->target_temperature = DEFAULT_TARGET_TEMPERATURE;
        this->current_temperature = 0.;
        this->done = false;
        this->selected = 0;
        this->start_time = 0;
    };
    
    Step(uint8_t step_number, uint16_t target_duration, uint16_t target_temperature) 
    {
        this->step_number = step_number;
        this->target_duration = target_duration;
        this->elapsed_duration = 0;
        this->target_temperature = target_temperature;
        this->current_temperature = 0.;
        this->done = false;
        this->selected = 0;
        this->start_time = 0;
    };

    void setStepNumber(uint8_t step_number) { this->step_number = step_number; };

    void updateElapsedTime(unsigned long millis);
    
    void setCurrentTemperature(float current_temperature) { this->current_temperature = current_temperature; };

    boolean next() 
    { 
        
        if (selected < MAX_SELECTOR) {
            selected++;         
            return false;
        }
        return true;
    }
    boolean prev() 
    {       
        if (selected > 0) {
            selected--;
            return false;
        }
        return true;
    }

    void inc();
    void dec();

    boolean isOn() 
    {
        return (!isDone() && current_temperature < target_temperature);
    };

    void display(Adafruit_SSD1306 &display, boolean isRunning);
    boolean isDone() { return this->done; };

};

#endif
