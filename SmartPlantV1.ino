/*
 *   Smart Plant Pot by Lewis at DIY Machines
 *
 *   Find the build instructions and CAD data here: https://youtu.be/T_tpKoNCVYw
 *   My Youtube channel: www.youtube.com/c/diymachines
 *
 *   List of items used in this project and where to find them (the links are Amazon affiliate links and help support this channel at no cost to you):

■ Arduino Nano: https://geni.us/ArduinoNanoV3
■ Mini submersible pump: https://geni.us/MiniPump
■ 5mm tubing: https://geni.us/5mmTubing
■ Transistor: https://geni.us/2npn2222
■ Resistors (1k and 4.7k): https://geni.us/Ufa2s
■ Wire: https://geni.us/22AWGWire
■ 3mm LED: https://geni.us/LEDs
■ Water level sensor: https://geni.us/WaterLevelSensor
■ Nuts and Bolts: http://geni.us/NutsAndBolts
■ Soil moisture sensor: https://geni.us/MoistureSensor
■ Half Perma-proto board: https://geni.us/HalfPermaProto
■ PLA Filament: https://geni.us/PLAFilament

■ Battery powered glue gun: http://geni.us/BoschBatteryGlueGun
■ Battery powered soldering iron: http://bit.ly/SealeySDL6


 * Modified by @SFrijters for use with a different soil moisture sensor and make the code more chatty.
 *
 * Moisture sensor: https://www.az-delivery.de/nl/products/bodenfeuchte-sensor-modul-v1-2
 */

// These constants won't change. They're used to give names to the pins used:
const int ledPin = 2;                              // Digital output pin that the LED is attached to
const int pumpPin = 12;                            // Digital output pin that the water pump is attached to
const int waterLevelPin = A3;                      // Analog pin water level sensor is connected to
const int moistureSensorPin = A0;                  // Analog input pin used to check the moisture level of the soil

// Startup
const int timesStartupFlash = 5;                   // How often the LED will flash at startup
const long timeStartupFlash_ms = 600;              // How long a LED flash cycle at startup lasts

// Water level and LED
const int waterLevelSensorMinValue = 30;           // The value that the water level sensor returns when it is (almost) dry - at least no longer submerged but maybe still drying in the air
const int waterLevelSensorMaxValue = 133;          // The value that the water level sensor returns when it is fully submerged
const int waterLevelWarningPercentThreshold = 25;  // Threshold at which we flash the LED to warn you of a low water level in the pump tank
const int waterLevelStopPumpPercentThreshold = 5;  // Threshold at which we stop trying to pump water
const int timesWaterLevelWarningFlash = 5;         // How often the LED will flash to tell us the water tank needs topping up
const long timeWaterLevelWarningFlash_ms = 2000;   // How long a LED flash cycle to warn about an empty reservoir lasts
const int timesWaterLevelStopPumpFlash = 5;        // How often the LED will flash to tell us the water tank needs topping up
const long timeWaterLevelStopPumpFlash_ms = 200;   // How long a LED flash cycle to warn about an empty reservoir lasts

// Soil moisture and pump
const int moistureSensorAirValue = 450;            // When it is dried and held in air, it's the minimum moisture
const int moistureSensorWaterValue = 120;          // When it is submerged in water, it's the maximum moisture
const int moistureSoilPercentThreshold = 5;        // At which soil moisture percentage the pump should be activated
const long timeToPump_ms = 2500;                   // How long the pump should pump water for when the plant needs it
const long timeToAllowMoistureSpread_ms = 2000;    // How long moisture is allowed to spread in the soil after pumping

// Sleeping
const unsigned long cycleInterval_ms = 36000000;   // Cycle is one hour
const unsigned long sleepInterval_ms = 10000;      // Time to wait aftr checking the cycle is 10 seconds.

int waterLevelSensorValue = 0;  // Somewhere to store the value read from the waterlevel sensor
int waterLevelPercent = 0;
int moistureSensorValue = 0;    // Somewhere to store the value read from the soil moisture sensor
int moistureSoilPercent = 0;

unsigned long lastMillis;

typedef enum {
  okState,
  waterLevelLowState,
  activeState,
  nProgramStates,
} programState;

programState _programState = okState;

void setup() {
    // Set the BAUD rate
    Serial.begin(9600);

    Serial.println("Starting up");

    // Set the operational mode for the pins
    pinMode(ledPin, OUTPUT);
    pinMode(pumpPin, OUTPUT);
    pinMode(waterLevelPin, INPUT);
    pinMode(moistureSensorPin, INPUT);

    lastMillis = millis();

    Serial.println("Pins set");

    // Flash the LED five times to confirm power on and operation of code:
    for (int i = 0; i < timesStartupFlash; i++) {
        digitalWrite(ledPin, HIGH);
        delay(timeStartupFlash_ms / 2);
        digitalWrite(ledPin, LOW);
        delay(timeStartupFlash_ms / 2);
    }

    // Turn the LED on
    digitalWrite(ledPin, HIGH);

    Serial.println("Startup done");
}

void readWaterLevelPercent() {
    Serial.println("Reading water level");
    waterLevelSensorValue = analogRead(waterLevelPin);
    waterLevelPercent = map(waterLevelSensorValue,
                            waterLevelSensorMinValue, waterLevelSensorMaxValue,
                            0, 100);

    // Set waterLevelPercent to minimum 0% and maximum 100%
    if (waterLevelPercent > 100) {
        waterLevelPercent = 100;
    } else if (waterLevelPercent < 0) {
        waterLevelPercent = 0;
    }

    Serial.print("  Water level sensor value: ");
    Serial.print(waterLevelSensorValue);
    Serial.print(" -> ");
    Serial.print(waterLevelPercent);
    Serial.print("%");
}

void doWaterLevelStopPump() {
    Serial.print(" < ");
    Serial.print(waterLevelStopPumpPercentThreshold);
    Serial.println("%");
    Serial.println("  Water level too low, refusing to pump, blinking to get your attention");
    for (int i = 0; i < timesWaterLevelStopPumpFlash; i++) {
        digitalWrite(ledPin, LOW);
        delay(timeWaterLevelStopPumpFlash_ms / 2);
        digitalWrite(ledPin, HIGH);
        delay(timeWaterLevelStopPumpFlash_ms / 2);
    }
}

void doWaterLevelWarning() {
    Serial.print(" < ");
    Serial.print(waterLevelWarningPercentThreshold);
    Serial.println("%");
    Serial.println("  Water level getting low, blinking to get your attention");
    for (int i = 0; i < timesWaterLevelWarningFlash; i++) {
        digitalWrite(ledPin, LOW);
        delay(timeWaterLevelWarningFlash_ms / 2);
        digitalWrite(ledPin, HIGH);
        delay(timeWaterLevelWarningFlash_ms / 2);
    }
}

void doWaterLevelOk() {
    Serial.print(" > ");
    Serial.print(waterLevelWarningPercentThreshold);
    Serial.println("%");
    Serial.println("  Water level okay");
}

void readMoistureSoilPercent() {
    Serial.println("Reading moisture level");
    moistureSensorValue = analogRead(moistureSensorPin);
    moistureSoilPercent = map(moistureSensorValue,
                              moistureSensorAirValue, moistureSensorWaterValue,
                              0, 100);
    // Set moistureSoilPercent to minimum 0% and maximum 100%
    if (moistureSoilPercent > 100) {
        moistureSoilPercent = 100;
    } else if (moistureSoilPercent < 0) {
        moistureSoilPercent = 0;
    }

    Serial.print("  Moisture sensor value: ");
    Serial.print(moistureSensorValue);
    Serial.print(" -> ");
    Serial.print(moistureSoilPercent);
    Serial.print("%");
}

void doPumpWater() {
    Serial.print(" < ");
    Serial.print(moistureSoilPercentThreshold);
    Serial.println("%");
    digitalWrite(pumpPin, HIGH);
    Serial.print("    Pumping for ");
    Serial.print(timeToPump_ms);
    Serial.println(" milliseconds");
    delay(timeToPump_ms);
    digitalWrite(pumpPin, LOW);
    Serial.println("    Done pumping");

    // Delay to allow the moisture in the soil to spread through to the sensor
    Serial.print("    Waiting for moisture to spread for ");
    Serial.print(timeToAllowMoistureSpread_ms);
    Serial.println(" milliseconds");
    delay(timeToAllowMoistureSpread_ms);
}

void doNotPumpWater() {
    Serial.print(" > ");
    Serial.print(moistureSoilPercentThreshold);
    Serial.println("%");
}

void doThings() {
    readWaterLevelPercent();

    if (waterLevelPercent < waterLevelStopPumpPercentThreshold) {
        doWaterLevelStopPump();
        // Early return: we cannot act on anything if the water level is too low
        return;
    }

    if (waterLevelPercent < waterLevelWarningPercentThreshold) {
        doWaterLevelWarning();
    } else {
        doWaterLevelOk();
    }

    readMoistureSoilPercent();

    // If the soil is too dry, activate the pump
    if (moistureSoilPercent < moistureSoilPercentThreshold) {
        doPumpWater();
    } else {
        doNotPumpWater();
        if (waterLevelPercent < waterLevelWarningPercentThreshold) {
            _programState = waterLevelLowState;
        } else {
            _programState = okState;
        }
    }
}

void loop() {
    // Keep interacting with the pot until were are in a stable state
    _programState = activeState;
    while (_programState != okState) {
        doThings();
    }

    // If everything is (finally) okay, sleep for a while
    // NOTE: This will overflow after ~50 days but I don't care.
    // https://www.arduino.cc/reference/en/language/functions/time/millis/
    while (millis() - lastMillis < cycleInterval_ms) {
        Serial.print("Sleeping for ");
        Serial.print(sleepInterval_ms);
        Serial.println(" milliseconds");
        delay(sleepInterval_ms);
    }
    lastMillis = millis();
}

// Local Variables:
// mode: c
// c-basic-offset: 4
// c-file-style: "stroustrup"
// indent-tabs-mode: nil
// End:
