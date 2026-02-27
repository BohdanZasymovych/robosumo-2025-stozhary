#include <Arduino.h>
#include "linesensor.h"


Linesensor::Linesensor(uint8_t pin) : pin(pin) {
        pinMode(pin, INPUT);
}

bool Linesensor::isLineDetected() {
        return digitalRead(pin);
}
