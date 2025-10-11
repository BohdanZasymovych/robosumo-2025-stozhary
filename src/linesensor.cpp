#include <Arduino.h>
#include "linesensor.h"


Linesensor::Linesensor(uint8_t pin) : pin(pin) {
        Serial.begin(115200);
        pinMode(pin, INPUT);
}

bool Linesensor::isLineDetected() {
        Serial.println(digitalRead(pin));
        return true;
}
