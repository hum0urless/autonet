#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>  

Preferences preferences;
WebServer server(80);

const char *apSSID = "ESP32_Setup";  

void handleRoot() {
    server.send(200, "text/html", "<form action='/save' method='POST'>"
                                  "SSID: <input name='ssid'><br>"
                                  "Password: <input type='password' name='pass'><br>"
                                  "<input type='submit' value='Save'></form>");
}

void handleSave() {
    String ssid = server.arg("ssid");
    String pass = server.arg("pass");

    if (ssid.length() > 0 && pass.length() > 0) {
        preferences.begin("wifi", false);
        preferences.putString("s", ssid);
        preferences.putString("p", pass);
        preferences.end();
        server.send(200, "text/html", "Saved! Rebooting...");
        delay(2000);
        ESP.restart();
    } else {
        server.send(400, "text/html", "Invalid input");
    }
}

void setup() {
    Serial.begin(115200);
    preferences.begin("wifi", true);
    String s = preferences.getString("s", "");
    String p = preferences.getString("p", "");
    preferences.end();

    if (s.length() > 0) {
        WiFi.begin(s.c_str(), p.c_str());
        for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) delay(500);
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println(WiFi.localIP());
            return;
        }
    }

    WiFi.softAP(apSSID);
    Serial.println(WiFi.softAPIP());
    
    server.on("/", handleRoot);
    server.on("/save", HTTP_POST, handleSave);
    server.begin();
}

void loop() {
    server.handleClient();
}
