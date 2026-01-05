#include <Arduino.h>
#include "motor.h"
#include "linesensor.h"
#include "ultrasonic.h"
#include "robot.h"
#include "vl53l5cx.h"

// constexpr int in1Motor1 = 46;
// constexpr int in2Motor1 = 3;
// constexpr int enableMotor1 = 9;

// constexpr int in1Motor2 = 8;
// constexpr int in2Motor2 = 18;
// constexpr int enableMotor2 = 17;


// Motor rightMotor = Motor(in1Motor1, in2Motor1, enableMotor1);
// Motor leftMotor = Motor(in1Motor2, in2Motor2, enableMotor2);
// Robot robot(leftMotor, rightMotor, 130);

#define SDA_PIN 20
#define SCL_PIN 21
#define INT_PIN 4

#define VL53L5CX_MEASURING_FREQUENCY 60

void printObjectMatrix();

VL53L5CX sensor = VL53L5CX(SCL_PIN, SDA_PIN, INT_PIN);
is_object_detected_matrix isObjectDetectedMatrix;


void setup() {
    Serial.begin(115200);
    if (!sensor.begin(VL53L5CX_MEASURING_FREQUENCY)) {
        Serial.println("Sensor initialization failed");
        while (true) {}
    }
}

void loop() {
    sensor.getData(isObjectDetectedMatrix);
    printObjectMatrix();
    // sensor.printDistanceMatrix();
}

void printObjectMatrix() {
    for (int i = 0; i < IMAGE_SIDE_SIZE; ++i) {
        for (int j = 0; j < IMAGE_SIDE_SIZE; ++j) {
          Serial.print(isObjectDetectedMatrix[i][j]);
          Serial.print("\t");
        }
        Serial.println();
      }
    Serial.println("-----------------------------------------");
}
