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
// VL53L1X FRONT SENSORS 
// ========================================

// Left Sensor
#define VL53L1X_LEFT_INT_PIN    4
#define VL53L1X_LEFT_XSHUT_PIN  5
#define VL53L1X_LEFT_ADDRESS    0x30

// Center Sensor
#define VL53L1X_CENTER_INT_PIN  8
#define VL53L1X_CENTER_XSHUT_PIN 9
#define VL53L1X_CENTER_ADDRESS  0x31
 
// Right Sensor
#define VL53L1X_RIGHT_INT_PIN   13
#define VL53L1X_RIGHT_XSHUT_PIN 14
#define VL53L1X_RIGHT_ADDRESS   0x32


// ========================================
// VL53L0X SIDE SENSORS 
// ========================================

// Left Sensor
#define VL53L0X_LEFT_INT_PIN    4
#define VL53L0X_LEFT_XSHUT_PIN  5
#define VL53L0X_LEFT_ADDRESS    0x33
 
// Right Sensor
#define VL53L0X_RIGHT_INT_PIN   13
#define VL53L0X_RIGHT_XSHUT_PIN 14
#define VL53L0X_RIGHT_ADDRESS   0x34


// ========================================
// LINE SENSORS
// ========================================


// ========================================
// LADLE
// ========================================

// Servo Motors
#define LADLE_SERVO1_PIN 20
#define LADLE_SERVO2_PIN 21

// Force Sensors (датчики сили/тиску)
#define FORCE_SENSOR1_PIN 48  // Верхній: старт 0-100, натиск >= 170
#define FORCE_SENSOR2_PIN 47  // Нижній: 0=немає, натиск >= 60


#endif // PINOUT_H
