#ifndef PINOUT_H
#define PINOUT_H


// ========================================
// MOTOR PINS
// ========================================

// Left Motor
#define MOTOR_LEFT_LPWM 1
#define MOTOR_LEFT_RPWM 2

// Right Motor
#define MOTOR_RIGHT_LPWM 6
#define MOTOR_RIGHT_RPWM 7


// ========================================
// I2C Wires
// ========================================

// Wire
#define I2C_WIRE_SCL 11
#define I2C_WIRE_SDA 12
#define I2C_WIRE_CLOCK_FREQ 400000

// Wire1
#define I2C_WIRE1_SCL 13
#define I2C_WIRE1_SDA 14
#define I2C_WIRE1_CLOCK_FREQ 400000


// ========================================
// VL53L1X FRONT SENSORS 
// ========================================

// Left Sensor
#define VL53L1X_LEFT_INT_PIN    35
#define VL53L1X_LEFT_XSHUT_PIN  36
#define VL53L1X_LEFT_ADDRESS    0x30

// Center Sensor
#define VL53L1X_CENTER_INT_PIN  37
#define VL53L1X_CENTER_XSHUT_PIN 38
#define VL53L1X_CENTER_ADDRESS  0x31
 
// Right Sensor
#define VL53L1X_RIGHT_INT_PIN   39
#define VL53L1X_RIGHT_XSHUT_PIN 40
#define VL53L1X_RIGHT_ADDRESS   0x32


// ========================================
// VL53L0X SIDE SENSORS 
// ========================================

// Left Sensor
#define VL53L0X_LEFT_INT_PIN    15
#define VL53L0X_LEFT_XSHUT_PIN  16
#define VL53L0X_LEFT_ADDRESS    0x33
 
// Right Sensor
#define VL53L0X_RIGHT_INT_PIN   17
#define VL53L0X_RIGHT_XSHUT_PIN 18
#define VL53L0X_RIGHT_ADDRESS   0x34


// ========================================
// LINE SENSORS
// ========================================

#define LINE_SENSOR_LEFT 42
#define LINE_SENSOR_RIGHT 4

#endif // PINOUT_H
