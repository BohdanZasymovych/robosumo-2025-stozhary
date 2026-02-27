#ifndef PINOUT_H
#define PINOUT_H


// ========================================
// MOTOR PINS
// ========================================

// Right Motor
#define MOTOR_RIGHT_LPWM 9
#define MOTOR_RIGHT_RPWM 8

// Left Motor
#define MOTOR_LEFT_LPWM 6
#define MOTOR_LEFT_RPWM 7


// ========================================
// I2C Wires
// ========================================

// Wire
#define I2C_WIRE_SCL 40
#define I2C_WIRE_SDA 41
#define I2C_WIRE_CLOCK_FREQ 400000

// Wire1
#define I2C_WIRE1_SCL 20
#define I2C_WIRE1_SDA 21
#define I2C_WIRE1_CLOCK_FREQ 400000


// ========================================
// FRONT SENSORS (VL53L0X left/right, VL53L1X center)
// ========================================

// Left Sensor VL53L0X
#define FRONT_LEFT_INT_PIN      4
#define FRONT_LEFT_XSHUT_PIN    5
#define FRONT_LEFT_ADDRESS      0x30

// Center Sensor VL53L1X
#define FRONT_CENTER_INT_PIN    2
#define FRONT_CENTER_XSHUT_PIN  3
#define FRONT_CENTER_ADDRESS    0x31
 
// Right Sensor VL53L0X
#define FRONT_RIGHT_INT_PIN     10
#define FRONT_RIGHT_XSHUT_PIN   11
#define FRONT_RIGHT_ADDRESS     0x32


// ========================================
// SIDE SENSORS (VL53L0X)
// ========================================

// Left Side Sensor
#define SIDE_LEFT_INT_PIN       12
#define SIDE_LEFT_XSHUT_PIN     13
#define SIDE_LEFT_ADDRESS       0x33
 
// Right Side Sensor
#define SIDE_RIGHT_INT_PIN      14
#define SIDE_RIGHT_XSHUT_PIN    15
#define SIDE_RIGHT_ADDRESS      0x34


// ========================================
// LINE SENSORS
// ========================================

#define LINE_SENSOR_LEFT 42
#define LINE_SENSOR_RIGHT 4


// ========================================
// LADLE
// ========================================

// Servo Motors
#define LADLE_SERVO1_PIN 20
#define LADLE_SERVO2_PIN 21

// Force Sensors
#define FORCE_SENSOR1_PIN 48
#define FORCE_SENSOR2_PIN 47


#endif // PINOUT_H