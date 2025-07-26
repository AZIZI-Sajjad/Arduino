#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
extern "C" {
  #include "user_interface.h"  // Nécessaire pour wifi_softap_get_station_info
}

// Wi-Fi configuration
const char* ssid_sta = "SSID_EXISTANT";
const char* password_sta = "MOT_DE_PASSE_EXISTANT";

const char* ssid_ap = "Hotspot_ESP8266";
const char* password_ap = "motdepasseap";  // min 8 chars

ESP8266WebServer server(80);  // HTTP server

void setup() {
  Serial.begin(115200);

  // Dual mode: Station (STA) + Access Point (AP)
  WiFi.mode(WIFI_AP_STA);

  // Connect to existing Wi-Fi (STA mode)
  WiFi.begin(ssid_sta, password_sta);
  Serial.println("Connecting to Wi-Fi (STA)...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to STA. IP: ");
  Serial.println(WiFi.localIP());

  // Start Access Point (AP mode)
  if (WiFi.softAP(ssid_ap, password_ap)) {
    Serial.println("Access Point started!");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("Failed to start AP.");
  }

  // Define web page route
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server running.");
}

void loop() {
  server.handleClient();
}

// Serve HTML status page
void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<title>ESP8266 Hotspot Monitor</title></head><body>";
  html += "<h1>ESP8266 Access Point Monitor</h1>";

  html += "<h2>System Info</h2>";
  html += "<ul>";
  html += "<li>Uptime: " + String(millis() / 1000) + " seconds</li>";
  html += "<li>Free heap: " + String(ESP.getFreeHeap()) + " bytes</li>";
  html += "<li>RSSI (STA): " + String(WiFi.RSSI()) + " dBm</li>";
  html += "</ul>";

  html += "<h2>Connected Clients (AP)</h2><ul>";

  struct station_info* station = wifi_softap_get_station_info();
  if (station == nullptr) {
    html += "<li>No clients connected</li>";
  } else {
    while (station) {
      IPAddress ip = IPAddress(station->ip.addr);
      html += "<li>Client IP: " + ip.toString() + "</li>";
      station = STAILQ_NEXT(station, next);
    }
  }
  wifi_softap_free_station_info();  // free memory

  html += "</ul>";
  html += "<p><em>Page auto-refresh every 10s</em></p>";
  html += "<script>setTimeout(() => location.reload(), 10000);</script>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}
