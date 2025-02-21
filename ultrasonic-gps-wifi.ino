#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

#define TRIG 25
#define ECHO 26

const char* ssid = "Airtel_hrid_ayus7333";
const char* password = "<PASSWORD>";
const char* serverURL = "http://192.168.1.3:5000/data";

TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

long measureDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH, 30000);
  return (duration > 0) ? (duration * 0.034 / 2) : -1;
}

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, 35, 32); // RX=35, TX=32
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void loop() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }
  
  long distance = measureDistance();
  double latitude = gps.location.isValid() ? gps.location.lat() : 0.0;
  double longitude = gps.location.isValid() ? gps.location.lng() : 0.0;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm, GPS: ");
  Serial.print(latitude, 6);
  Serial.print(", ");
  Serial.println(longitude, 6);

  HTTPClient http;
  http.begin(serverURL);
  http.addHeader("Content-Type", "application/json");
  String jsonPayload = "{\"distance\": " + String(distance) + ", \"latitude\": " + String(latitude, 6) + ", \"longitude\": " + String(longitude, 6) + "}";
  int httpResponseCode = http.POST(jsonPayload);
  if (httpResponseCode > 0) {
    Serial.println("Data sent successfully");
  } else {
    Serial.print("Error sending data: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  delay(1000);
}
