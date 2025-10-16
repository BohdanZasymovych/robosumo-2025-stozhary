#include <Arduino.h>
#include "motor.h"
#include "linesensor.h"
#include "ultrasonic.h"

constexpr int in1Motor1 = 46;
constexpr int in2Motor1 = 3;
constexpr int enableMotor1 = 9;

constexpr int in1Motor2 = 8;
constexpr int in2Motor2 = 18;
constexpr int enableMotor2 = 17;

constexpr int linesensorPin = 16;

Linesensor l1 = Linesensor(linesensorPin);

constexpr int trig = 11;
constexpr int echo = 10;

Motor m1 = Motor(in1Motor1, in2Motor1, enableMotor1);
Motor m2 = Motor(in1Motor2, in2Motor2, enableMotor2);
Ultrasonic ultrasonic = Ultrasonic(trig, echo);

void setup() {
    Serial.begin(115200);

    delay(5000);
}

void loop() {
    long distance = ultrasonic.getDistance();
    Serial.println(distance);

    if (distance == 150) {
        m1.move(75);
        m2.move(-75);
        return;
    }

    m1.move(100);
    m2.move(100);
}
