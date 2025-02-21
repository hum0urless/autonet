#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define ENA 18  // PWM Speed Control
#define IN1 19  // Motor Direction 1
#define IN2 21  // Motor Direction 2

// WiFi Credentials
const char* ssid = "Airtel_103-air";
const char* password = "<PASSWORD>";
const char* server_url = "http://192.168.1.7:5000/motor"; // Replace with Flask server IP

void setup() {
    Serial.begin(115200);

    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected!");
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(server_url);
        
        int httpResponseCode = http.GET();
        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("Received: " + response);

            // Parse JSON response
            DynamicJsonDocument doc(256);
            deserializeJson(doc, response);
            
            String direction = doc["direction"];
            int speed = doc["speed"];

            // Apply motor control logic
            if (direction == "forward") {
                moveForward(speed);
            } else if (direction == "reverse") {
                moveReverse(speed);
            } else {
                stopMotor();
            }
        } else {
            Serial.println("Error fetching motor state.");
        }
        http.end();
    }
    delay(1000);  // Fetch command every second
}

void moveForward(int speed) {
    Serial.println("Moving Forward");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speed);  // Adjust speed (0-255)
}

void moveReverse(int speed) {
    Serial.println("Moving Reverse");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, speed);
}

void stopMotor() {
    Serial.println("Stopping");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
}