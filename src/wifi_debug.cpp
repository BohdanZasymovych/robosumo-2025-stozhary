#include "wifi_debug.h"
#include <cstdarg>

WiFiServer* WiFiDebug::server = nullptr;
WiFiClient  WiFiDebug::client;
uint16_t    WiFiDebug::serverPort = 2323;
bool        WiFiDebug::mirrorSerial = true;

bool WiFiDebug::begin(const char* ssid,
                      const char* pass,
                      uint16_t port,
                      uint32_t wifiTimeoutMs,
                      bool mirrorToUsbSerial)
{
  mirrorSerial = mirrorToUsbSerial;
  serverPort = port;

  if (mirrorSerial) {
    // Якщо Serial уже стартуєш у main — це не заважає
    if (!Serial) { /* no-op */ }
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - start) < wifiTimeoutMs) {
    delay(250);
    if (mirrorSerial) Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {
    if (mirrorSerial) Serial.println("\n[WiFiDebug] WiFi connect TIMEOUT");
    return false;
  }

  if (mirrorSerial) {
    Serial.println("\n[WiFiDebug] WiFi connected!");
    Serial.print("[WiFiDebug] IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("[WiFiDebug] TCP port: ");
    Serial.println(serverPort);
  }

  // Створюємо сервер
  if (server) {
    delete server;
    server = nullptr;
  }
  server = new WiFiServer(serverPort);
  server->begin();
  server->setNoDelay(true);

  return true;
}

void WiFiDebug::update() {
  if (!server) return;

  // Якщо клієнт відвалився — закриваємо
  if (client && !client.connected()) {
    client.stop();
  }

  // Якщо клієнта нема — пробуємо прийняти нового
  if (!client || !client.connected()) {
    WiFiClient newClient = server->available();
    if (newClient) {
      client = newClient;
      client.setNoDelay(true);

      // Привітання
      client.println("Connected to ESP32-S3 WiFiDebug!");
      client.print("IP: ");
      client.println(WiFi.localIP());
      client.print("Port: ");
      client.println(serverPort);
    }
  }
}

void WiFiDebug::print(const char* s) {
  if (mirrorSerial) Serial.print(s);
  if (client && client.connected()) client.print(s);
}

void WiFiDebug::println(const char* s) {
  if (mirrorSerial) Serial.println(s);
  if (client && client.connected()) client.println(s);
}

void WiFiDebug::printf(const char* fmt, ...) {
  char buf[256];

  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);

  print(buf);
}

IPAddress WiFiDebug::ip() {
  return WiFi.localIP();
}

bool WiFiDebug::connected() {
  return (client && client.connected());
}

void WiFiDebug::closeClient() {
  if (client) client.stop();
}


bool WiFiDebug::beginAP(const char* apName) {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(apName);

    IPAddress IP = WiFi.softAPIP();

    if (mirrorSerial) {
      Serial.println("[WiFiDebug] AP started!");
      Serial.print("[WiFiDebug] AP IP: ");
      Serial.println(IP);
      Serial.print("[WiFiDebug] TCP port: ");
      Serial.println(serverPort);
    }

    if (server) { delete server; server = nullptr; }
    server = new WiFiServer(serverPort);
    server->begin();
    server->setNoDelay(true);

    return true;
}
