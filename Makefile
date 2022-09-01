SHELL=/usr/bin/env bash
SERIAL=/dev/ttyUSB0
BAUDRATE=9600
PROFILE=SmartPlantV1

.PHONY: all compile upload monitor

all: compile upload

compile:
	arduino-cli compile -v \
	--profile $(PROFILE) \
	--build-property "compiler.cpp.extra_flags=\"-DGIT_VERSION=\"$(shell git rev-parse HEAD)\"\"" \
	--build-property "runtime.tools.python3.path=$(_ARDUINO_PYTHON3)/bin"

upload:
	[ -e $(SERIAL) ] && \
	arduino-cli upload -v -p $(SERIAL) --profile $(PROFILE) || \
	{ echo "==> $(SERIAL) is not available"; exit 1; }

monitor:
	picocom -b $(BAUDRATE) --imap lfcrlf $(SERIAL)
