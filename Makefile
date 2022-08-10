SHELL=/usr/bin/env bash
SERIAL=/dev/ttyUSB0
BAUDRATE=9600

.PHONY: all

all: compile upload

compile:
	arduino-cli compile -v --fqbn $(_ARDUINO_FQBN) SmartPlantV1

upload:
	[ -e $(SERIAL) ] && arduino-cli upload -v -p $(SERIAL) --fqbn $(_ARDUINO_FQBN) SmartPlantV1 || { echo "==> $(SERIAL) is not available"; exit 1; }

update:
	arduino-cli core update-index

screen:
	screen $(SERIAL) $(BAUDRATE)
