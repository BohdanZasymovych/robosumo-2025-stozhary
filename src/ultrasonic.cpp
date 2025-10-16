#include <Arduino.h>
#include "ultrasonic.h"


Ultrasonic::Ultrasonic(const uint8_t trig, const uint8_t echo) : trigPin(trig), echoPin(echo) {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

unsigned int Ultrasonic::getDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    unsigned long duration = pulseIn(echoPin, HIGH, 100000UL);

    unsigned int distance = (duration * 0.0343) / 2;

    if (distance == 0) {
        return 150;
    }

    return distance;
}