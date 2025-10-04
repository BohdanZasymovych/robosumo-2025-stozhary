#include <Arduino.h>
#include "motor.h"

constexpr int in1Motor1 = 46;
constexpr int in2Motor1 = 3;
constexpr int enableMotor1 = 9;

constexpr int in1Motor2 = 8;
constexpr int in2Motor2 = 18;
constexpr int enableMotor2 = 17;

Motor m1 = Motor(in1Motor1, in2Motor1, enableMotor1);
Motor m2 = Motor(in1Motor2, in2Motor2, enableMotor2);

void setup() {
    m1.move(100);
    m2.move(100);
    
    delay(5000);

    m1.move(-100);
    m2.move(-100);
    
    delay(5000);    

    m1.move(100);
    m2.move(-100);

    m1.stop();
    m2.stop();
}

void loop() {

}
