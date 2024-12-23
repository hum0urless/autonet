#include <WiFi.h>

// Set your Access Point credentials
const char* ssid = "ESP32_AP";
const char* password = "12345678";

WiFiServer server(80); // Port 80 for HTTP
int ledPin = 2;        // Built-in LED (GPIO 2)

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(115200);
  Serial.println("Setting up Access Point...");

  // Start the access point
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Start the web server
  server.begin();
}

void loop() {
  WiFiClient client = server.available(); // Check if a client has connected
  if (client) {
    Serial.println("Client Connected");
    String request = client.readStringUntil('\r');
    client.flush();

    // Check for LED ON or OFF requests
    if (request.indexOf("/LED=ON") != -1) {
      digitalWrite(ledPin, HIGH);
    } else if (request.indexOf("/LED=OFF") != -1) {
      digitalWrite(ledPin, LOW);
    }

    // Send a response back to the client
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<h1>ESP32 Access Point</h1>");
    client.println("<p><a href=\"/LED=ON\">Turn LED ON</a></p>");
    client.println("<p><a href=\"/LED=OFF\">Turn LED OFF</a></p>");
    client.println("</html>");
    client.stop();
  }
}
