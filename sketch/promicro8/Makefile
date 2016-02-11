# Makefile for SparkFun ProMicro33(https://www.sparkfun.com/products/12587)
#
# show details for valiables in
# https://github.com/sudar/Arduino-Makefile/blob/master/arduino-mk-vars.md#compilerexecutable-variables
#
# library dependence
# * Arduino IDE v1.6.7
# * SparkFun Arduino Boards library v1.1.3 (https://github.com/sparkfun/Arduino_Boards)

BOARD_TAG = promicro
BOARD_SUB = 8MHzatmega32U4
USB_PID	= 0x9204
BOOTLOADER_PARENT = $(HOME)/Library/Arduino15/packages/SparkFun/hardware/avr/1.1.3/bootloaders
BOARDS_TXT = $(HOME)/Library/Arduino15/packages/SparkFun/hardware/avr/1.1.3/boards.txt
ARDUINO_VAR_PATH = /Users/ueha/Library/Arduino15/packages/SparkFun/hardware/avr/1.1.3/variants
MONITOR_PORT = /dev/cu.usb*
CXXFLAGS += -fno-threadsafe-statics

include $(ARDMK_DIR)/Arduino.mk
