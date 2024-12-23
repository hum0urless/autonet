#include <esp_now.h>
#include <WiFi.h>

#define TRIG_PIN 17 // Ultrasonic sensor TRIG pin
#define ECHO_PIN 19 // Ultrasonic sensor ECHO pin

uint8_t receiverMac[] = {0xC0, 0x5D, 0x89, 0xB1, 0x03, 0x48}; // Receiver's MAC address

float measureDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH); // Measure the time for echo
    float distance = duration * 0.034 / 2;   // Convert to cm
    return (distance > 0) ? distance : -1.0; // Return -1.0 if invalid
}

void setup() {
    Serial.begin(115200);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    WiFi.mode(WIFI_STA); // Set Wi-Fi to station mode

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, receiverMac, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }
}

void loop() {
    float distance = measureDistance();
    Serial.print("Sending Distance: ");
    Serial.println(distance);

    // Send distance as a float
    esp_err_t result = esp_now_send(receiverMac, (uint8_t *)&distance, sizeof(distance));

    if (result == ESP_OK) {
        Serial.println("Sent successfully");
    } else {
        Serial.println("Error sending the data");
    }

    delay(500); // Wait 500ms before sending again
}