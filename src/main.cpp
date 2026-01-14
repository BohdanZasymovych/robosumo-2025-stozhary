#include <Arduino.h>
#include "motor.h"
// #include "linesensor.h"
// #include "ultrasonic.h"
#include "robot.h"
#include "vl53l5cx.h"


void printObjectMatrix();
bool checkColumn(uint8_t rowIndex);
int8_t getObjectLocation();


#define in1Motor1 46
#define in2Motor1 3
#define enableMotor1 9

#define in1Motor2 8
#define in2Motor2 18
#define enableMotor2 17

Motor rightMotor = Motor(in1Motor1, in2Motor1, enableMotor1);
Motor leftMotor = Motor(in1Motor2, in2Motor2, enableMotor2);
Robot robot(leftMotor, rightMotor, 110);


#define SDA_PIN 20
#define SCL_PIN 21
#define INT_PIN 4

#define VL53L5CX_MEASURING_FREQUENCY 30

VL53L5CX sensor = VL53L5CX(SCL_PIN, SDA_PIN, INT_PIN);
is_object_detected_matrix isObjectDetectedMatrix;

#define NOTHING -2
#define LEFT -1
#define CENTER 0
#define RIGHT 1

int8_t prevObjectLocation = NOTHING;


void setup() {
    Serial.begin(115200);
    Serial.println("asdasdasdasdasdasd");
    if (!sensor.begin(VL53L5CX_MEASURING_FREQUENCY)) {
        Serial.println("Sensor initialization failed");
        while (true) {}
    }
    delay(3000);
}

void loop() {
    sensor.getData(isObjectDetectedMatrix);
    printObjectMatrix();

    int8_t objectLocation = getObjectLocation();

    // objectLocation = (objectLocation != NOTHING) ? objectLocation : prevObjectLocation;
    
    if (objectLocation == LEFT) {
        // robot.turnLeft();
        Serial.println("Left");
    } else if (objectLocation == RIGHT) {
        // robot.turnRight();
        Serial.print("Right");
    } else {
        // robot.stop();
        Serial.println("Center/Nothing");
    }
    
    prevObjectLocation = objectLocation;
    // Serial.println();
    // delay(500);
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

bool checkColumn(uint8_t colIndex) {
    return isObjectDetectedMatrix[0][colIndex] | isObjectDetectedMatrix[1][colIndex] | isObjectDetectedMatrix[2][colIndex];
}

int8_t getObjectLocation() {
    int8_t left = checkColumn(0) + checkColumn(1); 
    int8_t right = checkColumn(2) + checkColumn(3);

    if (!left & !right) {
        return NOTHING;
    }

    if (left > right) {
        return LEFT;
    } else if (left < right) {
        return RIGHT;
    }

    return CENTER;
}