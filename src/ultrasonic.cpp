#include <Arduino.h>
#include "ultrasonic.h"


Ultrasonic::Ultrasonic(const uint8_t trig, const uint8_t echo) : trigPin(trig), echoPin(echo) {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

unsigned int Ultrasonic::getDistance() {
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    unsigned int duration = pulseIn(echoPin, HIGH, 30000U);
    unsigned int distance = (duration * 0.0343) / 2;

    return distance;
}