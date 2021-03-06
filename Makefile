#
# AutoCooker_Uno_V0
# @author: Sebastien Soudan <sebastien.soudan@gmail.com>
#
ARDMK_DIR=../Arduino-Makefile/
ARDUINO_DIR=/Applications/Arduino-1.0.5.app/Contents/Resources/Java

CPPFLAGS=-DDEBUG

#NO_CORE = Yes

BOARD_TAG = uno

MONITOR_PORT = /dev/cu.usb*

ARDUINO_LIBS = Wire                 \
               Wire/utility         \
               TinkerKit            \
               MAX6675              \
               Adafruit-MCP23017    \
               AdafruitLCD          \
               Adafruit_SSD1306
include $(ARDMK_DIR)/arduino-mk/Arduino.mk

# !!! Important. You have to use make ispload to upload when using ISP programmer
