#include <WiFi.h>

void setup() {
    Serial.begin(115200);          // Start Serial Monitor
    WiFi.mode(WIFI_STA);           // Initialize Wi-Fi in station mode
    delay(1000);                   // Allow time for initialization
    Serial.println(WiFi.macAddress());  // Print MAC address
}

void loop() {
    // Nothing needed here
}
