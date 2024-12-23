#include <esp_now.h>
#include <WiFi.h>

#define LED_PIN 2 // Optional: Use an LED for visual feedback

void onReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
    Serial.print("Received from MAC: ");
    for (int i = 0; i < 6; i++) {
        Serial.printf("%02X", info->src_addr[i]);
        if (i < 5) Serial.print(":");
    }
    Serial.println();

    // Ensure the received data length matches the expected size for a float
    if (len == sizeof(float)) {
        float distance;
        memcpy(&distance, data, sizeof(float)); // Decode the float value
        Serial.print("Received Distance: ");
        Serial.println(distance);

        // Optional: Use LED for visual feedback if distance is below a threshold
        if (distance < 10.0) { // Example condition: distance below 10 cm
            digitalWrite(LED_PIN, HIGH);
        } else {
            digitalWrite(LED_PIN, LOW);
        }
    } else {
        Serial.println("Received data length mismatch");
    }
}

void setup() {
    Serial.begin(115200);

    // Optional: Set up an LED for visual feedback
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // Set Wi-Fi to station mode
    WiFi.mode(WIFI_STA);
    Serial.println("Wi-Fi set to Station mode");

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register callback function to handle incoming messages
    esp_now_register_recv_cb(onReceive);
    Serial.println("ESP-NOW initialized and callback registered");
}

void loop() {
    // No code required in loop for this example
}