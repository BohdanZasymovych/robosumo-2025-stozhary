#include <Arduino.h>
#include "motor.h"
#include "linesensor.h"
#include "ultrasonic.h"
#include "robot.h"

constexpr int in1Motor1 = 46;
constexpr int in2Motor1 = 3;
constexpr int enableMotor1 = 9;

constexpr int in1Motor2 = 8;
constexpr int in2Motor2 = 18;
constexpr int enableMotor2 = 17;

constexpr int linesensorPin = 16;

// Linesensor l1 = Linesensor(linesensorPin);

constexpr int trig = 11;
constexpr int echo = 10;

Motor leftMotor = Motor(in1Motor1, in2Motor1, enableMotor1);
Motor rightMotor = Motor(in1Motor2, in2Motor2, enableMotor2);
// Ultrasonic ultrasonic = Ultrasonic(trig, echo);
Robot robot(leftMotor, rightMotor, 150);
void setup() {
    Serial.begin(115200);
    Serial.println("Robot movement test starting in 5 seconds...");
    delay(5000);
}

void loop() {
    Serial.println("Moving forward...");
    robot.moveForward();   // їде вперед із базовою швидкістю (150)
    delay(1000);           // 3 секунди руху

    Serial.println("Stopping...");
    robot.stop();          // зупинка
    delay(1000);

    Serial.println("Moving BACKWARD...");
    robot.moveBackward();
    delay(1000);

    Serial.println("STOP");
    robot.stop();
    delay(1000);

    Serial.println("Turning LEFT...");
    robot.turnLeft();
    delay(2000);

    Serial.println("Turning RIGHT...");
    robot.turnRight();
    delay(2000);

    Serial.println("STOP");
    robot.stop();
    delay(2000);
    
    Serial.println("Test finished. Robot stopped.");
    while (true) {
        // зупиняємо програму, щоб робот не почав рухатись знову
    }
}
