#include "motor.h"
#include <Arduino.h>

class Motor {
    private:
        const uint8_t  forwardPin;
        const uint8_t  backwardPin;
        const uint8_t  analogPin;

    public:
        Motor(uint8_t forwardPin, uint8_t backwardPin, uint8_t analogPin) :
            forwardPin(forwardPin), backwardPin(backwardPin), analogPin(analogPin) {};

        turnOn() {
            
        }

};