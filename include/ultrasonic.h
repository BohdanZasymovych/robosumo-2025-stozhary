#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <cinttypes>


class Ultrasonic{
    const uint8_t trigPin;
    const uint8_t echoPin;

    public:
        Ultrasonic(const uint8_t trig, const uint8_t echo);
        unsigned int getDistance();
};

#endif