/*
 StepHolder.h
 
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
#ifndef __STEPHOLDER_H__
#define __STEPHOLDER_H__


#include "Step.h"

#ifndef ADAFRUIT_SSD1306__
#define ADAFRUIT_SSD1306__
    #include <Adafruit_GFX.h>
    #include <Adafruit_SSD1306.h>
#endif

#define MAX_STEPS 10

class StepHolder {

    private:
        Step steps[MAX_STEPS];
        uint8_t current_running_step;
        uint8_t current_edited_step;

        void moveToNextRunningStep() { if (this->current_running_step < MAX_STEPS - 1) this->current_running_step++; };
        void moveToPrevRunningStep() { if (this->current_running_step > 0) this->current_running_step--; };

        void moveToNextEditedStep() { if (this->current_edited_step < MAX_STEPS - 1) this->current_edited_step++; };
        void moveToPrevEditedStep() { if (this->current_edited_step > 0) this->current_edited_step--; };

    public:
        StepHolder() 
        {
            this->current_running_step = 0;
            this->current_edited_step = 0;

            for (uint8_t i = 0; i < MAX_STEPS; i++) 
            {
                steps[i].setStepNumber(i);
            }
        };
    
        void display(Adafruit_SSD1306 &display);

        void next() 
        { 
            if (steps[current_edited_step].next()) 
            {
                if (current_edited_step < MAX_STEPS - 1)
                    current_edited_step++;
            }
        };
        void prev() 
        { 
            if (steps[current_edited_step].prev())
            {
                if (current_edited_step > 0)
                    current_edited_step--;
            } 
        };

        void inc() { steps[current_edited_step].inc(); };
        void dec() { steps[current_edited_step].dec(); };

        void setCurrentTemperature(float current_temperature) 
        { 
            for (uint8_t i = 0; i < MAX_STEPS; i++) 
            {
                steps[i].setCurrentTemperature(current_temperature); 
            }
        };

        void updateElapsedTime(unsigned long millis) 
        { 
            steps[current_running_step].updateElapsedTime(millis);

            if (steps[current_running_step].isDone() && current_running_step < MAX_STEPS - 1) 
            {
                current_running_step++;
                // This time update the new running step too.
                steps[current_running_step].updateElapsedTime(millis);
            }
                
        };

        boolean isOn() {
            if (steps[current_running_step].isDone())
                return false;
            else 
                return steps[current_running_step].isOn();
        };
};

#endif
