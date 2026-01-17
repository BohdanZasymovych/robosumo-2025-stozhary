#include "motor.h"
#include <Arduino.h>


uint8_t Motor::nextChannel = 0; // Set starting PWM channel

Motor::Motor(uint8_t forwardPin, uint8_t backwardPin, uint8_t speedPin) :
    forwardPin(forwardPin), backwardPin(backwardPin), speedPin(speedPin), pwmChannel(nextChannel++) {
        pinMode(forwardPin, OUTPUT);
        pinMode(backwardPin, OUTPUT);
        pinMode(speedPin, OUTPUT);

        ledcSetup(pwmChannel, frequency, resolution);
        ledcAttachPin(speedPin, pwmChannel);  

        digitalWrite(forwardPin, LOW);
        digitalWrite(backwardPin, LOW);
        ledcWrite(pwmChannel, 0);
}

void Motor::stop() {
    digitalWrite(forwardPin, LOW);
    digitalWrite(backwardPin, LOW);
    ledcWrite(pwmChannel, 0);
}

void Motor::rotateForward(uint8_t speed) {
    digitalWrite(backwardPin, LOW);
    digitalWrite(forwardPin, HIGH);
    ledcWrite(pwmChannel, speed);
}

void Motor::rotateBackward(uint8_t speed) {
    digitalWrite(forwardPin, LOW);
    digitalWrite(backwardPin, HIGH);
    ledcWrite(pwmChannel, speed);
}