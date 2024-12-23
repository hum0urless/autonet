#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "Airtel_hrid_ayus7333";
const char* password = "darkchoco@312";

WiFiServer server(80);
int ledPin = 2; // Built-in LED for most ESP32 boards

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(115200);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client Connected");
    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("/LED=ON") != -1) {
      digitalWrite(ledPin, HIGH);
    } else if (request.indexOf("/LED=OFF") != -1) {
      digitalWrite(ledPin, LOW);
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<h1>ESP32 Web Server</h1>");
    client.println("<p><a href=\"/LED=ON\">Turn LED ON</a></p>");
    client.println("<p><a href=\"/LED=OFF\">Turn LED OFF</a></p>");
    client.println("</html>");
    client.stop();
  }
}
