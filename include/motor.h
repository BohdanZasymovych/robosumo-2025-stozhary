#ifndef MOTOR_H
#define MOTOR_H

#include <cinttypes>

class Motor {
    private:
        const uint8_t  forwardPin;
        const uint8_t  backwardPin;
        const uint8_t  analogPin;

    public:
        Motor(uint8_t forwardPin, uint8_t backwardPin, uint8_t analogPin);
        void turnOn();
        void turnOff();
        void setSpeed(uint8_t speed);
};

#endif
