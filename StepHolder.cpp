/*

 StepHolder.cpp

 Copyright (c) 2014 by Sebastien Soudan.  APL v2.

*/
#include "StepHolder.h"

void StepHolder::display(Adafruit_SSD1306 &display) {
    steps[current_edited_step].display(display, current_edited_step == current_running_step);
};

