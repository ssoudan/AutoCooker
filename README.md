# Arduino AutoCooker

Sebastien Soudan <sebastien.soudan@gmail.com>

# Description

This project is an Arduino-based cooking assistant that help you cook a meal multiple times at some given temperature.

You can find more details about it on my [blog](http://ssoudan.eu/posts/2014-12-22-arduino-bourguignon.html).

# Installation

This project uses a `Makefile` based on [sudar/Arduino-Makefile][https://github.com/sudar/Arduino-Makefile]. In order to build, you'll need that plus few libraries that are listed in the `Makefile`.

You can change `ARDMK_DIR` in the `Makefile` to make it point to the location where you have installed `sudar/Arduino-Makefile`.

In term of libraries, you'll need:

- Wire
- Wire/utility
- TinkerKit
- MAX6675
- Adafruit-MCP23017
- AdafruitLCD
- Adafruit_SSD1306

Once they have been installed you can build and upload the design to your board with:

    $ make
    $ make ispload

# License

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
