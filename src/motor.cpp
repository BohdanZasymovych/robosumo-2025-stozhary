#include "motor.h"
#include <Arduino.h>

uint8_t Motor::nextChannel = 0; // Set starting PWN channel

Motor::Motor(uint8_t forwardPin, uint8_t backwardPin, uint8_t speedPin) :
    forwardPin(forwardPin), backwardPin(backwardPin), speedPin(speedPin), pwmChannel(nextChannel++){
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

void Motor::move(int speed) {
    if (speed < 0) {
        digitalWrite(forwardPin, LOW);
        digitalWrite(backwardPin, HIGH);
        ledcWrite(pwmChannel, std::abs(speed));
    } else if (speed > 0) {
        digitalWrite(backwardPin, LOW);
        digitalWrite(forwardPin, HIGH);
        ledcWrite(pwmChannel, speed);
    } else {
        stop();
    }
}