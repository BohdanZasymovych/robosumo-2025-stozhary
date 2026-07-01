# Robosumo Robot "Stozhary"

Welcome to the repository for **Stozhary**, our autonomous mini-sumo robot designed for the 2025 RoboSumo competitions.

The robot is designed to quickly detect opponents using Time-of-Flight (ToF) sensors, stay inside the ring using line sensors, and utilize an active ladle mechanism to flip or push the opponent out of the ring!

## Hardware Components

Here is the hardware that brings the robot to life:

**Core & Locomotion:**
* **Microcontroller:** ESP32-S3
* **Motor Driver:** BTS7960
* **Motors:** 2x 5840-31ZY DC Motors
* **Wheels:** Custom 3D-printed wheels with high-friction custom silicone tires
* **Battery:** 3x 21700 Molicel P42A Li-ion cells (4200 mAh)

**Sensors & Actuators:**
* **Front Distance:** 1x VL53L1X (Center, Long Range) & 2x VL53L0X (Left/Right)
* **Side Distance:** 2x VL53L0X (Left/Right)
* **Ladle Distance:** 1x VL53L1X (Short Range)
* **Line Detectors:** 2x KY-033 Digital Line Sensors
* **Ladle Actuators:** 2x DC3218 Servo Motors
* **Ladle Force Sensors:** 2x Analog Force-Sensitive Resistors (FSRs) used to detect when the opponent is fully scooped

---

## Project Structure

The codebase is organized as a standard PlatformIO project:

* **`src/`**: Contains the main `.cpp` source files, where the actual logic and implementation of the classes live.
* **`include/`**: Contains all the `.h` header files. This is where classes, pinouts, and configurations are declared.
* **`lib/`**: Intended for custom or external private libraries (currently contains a README).
* **`test/`**: Reserved for unit testing using the PlatformIO Test Runner.
* **`platformio.ini`**: The configuration file containing board settings, upload speeds, and library dependencies (like Pololu's VL53L0X/VL53L1X libraries and ESP32Servo).

---

## Software Architecture & Classes

The code is heavily modularized into classes so that each piece of hardware has its own manager. Here is how the software works under the hood:

### 1. Movement & Control

* **`Motor`**: Handles low-level hardware control for the DC motors using ESP32's hardware PWM (`ledc`).
* **`Robot`**: A higher-level wrapper around the left and right motors. It handles complex movements like turning, moving forward/backward, and includes a clever "kickstart" timer to overcome initial motor inertia.

### 2. Sensing the Environment

* **`SensorVL53L0X` & `SensorVL53L1X`**: Base classes that manage the Time-of-Flight distance sensors. They use hardware interrupts (ISR) so the robot doesn't freeze up while waiting for a sensor to take a reading.
* **`FrontSensorArray` & `SideSensors`**: Managers that group multiple ToF sensors together. They continually update the main loop with the distances to any objects (opponents) around the robot.
* **`Linesensor`**: A simple class that reads the digital line sensors to check if the robot is near the white edge of the ring.

### 3. The Weapon: Ladle Mechanism

* **`Ladle`**: Controls the front scoop using two servo motors. It uses force sensors to detect physical contact and a dedicated ToF sensor (`LadleDistanceSensor`) to know when to strike. If it senses a sudden spike in pressure or an opponent right on top of it, it instantly lifts to flip them!

### 4. Utilities

* **`I2CBusManager`**: I2C buses can sometimes freeze due to electrical noise from motors. This class constantly monitors the `Wire` and `Wire1` buses. If a bus gets stuck, it automatically resets the pins and recovers the connection without crashing the robot.
* **`WiFiDebug`**: A handy utility that mirrors `Serial.print` logs over a wireless TCP connection. It allows us to read debug data in real-time while the robot is driving around the ring.

### 5. The Brain: `strategy.cpp`

This isn't a class, but a collection of logic functions that execute every loop:

* **Line Escape:** Has the highest priority. If a line is detected, it triggers a timed escape maneuver (straight back or curving away) to save the robot from falling out.
* **Attack:** If an opponent is seen by the front sensors, the robot charges at full speed (`SPEED_PUSH` or `SPEED_ATTACK`).
* **Search:** If no opponent is visible, the robot relies on its "memory" (where it last saw the enemy) or spins in place to find a target.
