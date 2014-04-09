UNAME = ${shell uname}
ifeq (${UNAME},Darwin)
	ARDUINO_DIR = /Applications/Arduino.app/Contents/Resources/Java
endif
ifeq (${UNAME},Linux)
	ARDUINO_DIR = /usr/share/arduino
endif

ARDMK_DIR = ~
BOARD_TAG = uno
TARGET = HHKBLite2BT
ARDUINO_LIBS = 
#MCU     = atmega328p
#F_CPU   = 16000000
ARDUINO_PORT = /dev/cu.usb*

ifeq (${UNAME},Darwin)
	ARDMK_PATH = ../Arduino-Makefile/bin
	ARDMK_DIR = ../Arduino-Makefile
	AVRDUDE_DIR = /usr/local/Cellar/avrdude/6.0.1
	AVR_TOOLS_DIR = $(ARDUINO_DIR)/hardware/tools/avr
	AVRDUDE = $(AVRDUDE_DIR)/bin/avrdude
	AVRDUDE_CONF = $(AVRDUDE_DIR)/etc/avrdude.conf
	include $(ARDMK_DIR)/Arduino.mk
endif
ifeq (${UNAME},Linux)
	include ${ARDUINO_DIR}/Arduino.mk
endif
