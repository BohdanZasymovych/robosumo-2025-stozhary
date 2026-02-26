#pragma once
#include <Arduino.h>
#include <WiFi.h>

/*
  WiFiDebug — простий WiFi логгер через TCP.
  Підключаєшся з ПК:  nc <ip> <port>   або  telnet <ip> <port>
*/

class WiFiDebug {
public:
  // Однією функцією ініціалізує WiFi + TCP сервер логів
  static bool begin(const char* ssid,
                    const char* pass,
                    uint16_t port = 2323,
                    uint32_t wifiTimeoutMs = 15000,
                    bool mirrorToUsbSerial = true);
    static bool beginAP(const char* apName = "ROBO_DEBUG");

  // Викликати в loop() — обробляє підключення/перепідключення клієнта
  static void update();

  // Логи
  static void print(const char* s);
  static void println(const char* s);
  static void printf(const char* fmt, ...);

  // Корисне
  static IPAddress ip();
  static bool connected();        // чи є TCP клієнт
  static void closeClient();      // від’єднати клієнта (якщо треба)

private:
  static WiFiServer* server;
  static WiFiClient client;
  static uint16_t serverPort;
  static bool mirrorSerial;



};



// Зручні макроси
#define WIFI_LOG(msg)        WiFiDebug::println(msg)
#define WIFI_PRINTF(fmt, ...) WiFiDebug::printf((fmt), ##__VA_ARGS__)
