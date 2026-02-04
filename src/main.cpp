#include <Arduino.h>
#include <ESP32Servo.h>

// Піни для серво моторів
#define SERVO1_PIN 9
#define SERVO2_PIN 10

// Створюємо два об'єкти серво
Servo servo1;
Servo servo2;

void setup() {
    // Ініціалізація серійного порту для виводу в термінал
    Serial.begin(115200);
    delay(1000); // Затримка для ініціалізації серійного порту

    // Підключаємо серво до пінів
    servo1.attach(SERVO1_PIN);
    servo2.attach(SERVO2_PIN);

    Serial.println("Тестування серво моторів розпочато");
    Serial.println("Серво 1 підключено до піна " + String(SERVO1_PIN));
    Serial.println("Серво 2 підключено до піна " + String(SERVO2_PIN));
}

void loop() {
    // Підйом серво на 90 градусів
    Serial.println("Підйом серво 1 та серво 2 на 90 градусів...");
    servo1.write(90);
    servo2.write(90);
    Serial.println("Серво піднято на 90 градусів");

    // Затримка 5 секунд
    Serial.println("Затримка 5 секунд...");
    delay(5000);

    // Опускаємо серво на 0 градусів
    Serial.println("Опускаємо серво 1 та серво 2 на 0 градусів...");
    servo1.write(0);
    servo2.write(0);
    Serial.println("Серво опущено на 0 градусів");

    // Затримка 5 секунд перед повторенням
    Serial.println("Затримка 5 секунд перед повторенням циклу...");
    delay(5000);
}