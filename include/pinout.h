#ifndef PINOUT_H
#define PINOUT_H


// ========================================
// MOTOR PINS
// ========================================

// Right Motor
#define MOTOR_RIGHT_FORWARD     46
#define MOTOR_RIGHT_BACKWARD    3
#define MOTOR_RIGHT_SPEED       9

// Left Motor
#define MOTOR_LEFT_FORWARD      8
#define MOTOR_LEFT_BACKWARD     18
#define MOTOR_LEFT_SPEED        17


// ========================================
// I2C Wires
// ========================================

// Wire
#define I2C_WIRE_SCL 20
#define I2C_WIRE_SDA 21
#define I2C_WIRE_CLOCK_FREQ 400000

// Wire1
#define I2C_WIRE1_SCL 20
#define I2C_WIRE1_SDA 21
#define I2C_WIRE1_CLOCK_FREQ 400000


// ========================================
// VL53L1X SENSORS (ToF Distance Sensors)
// ========================================

// Left Sensor
#define VL53L1X_LEFT_INT_PIN    5
#define VL53L1X_LEFT_XSHUT_PIN  6
#define VL53L1X_LEFT_ADDRESS    0x30

// Center Sensor
#define VL53L1X_CENTER_INT_PIN  7
#define VL53L1X_CENTER_XSHUT_PIN 10
#define VL53L1X_CENTER_ADDRESS  0x31

// Right Sensor
#define VL53L1X_RIGHT_INT_PIN   11
#define VL53L1X_RIGHT_XSHUT_PIN 12
#define VL53L1X_RIGHT_ADDRESS   0x32


// ========================================
// LINE SENSORS
// ========================================


#endif // PINOUT_H
