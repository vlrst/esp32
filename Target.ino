#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Booting...");

  WiFi.softAP("ESP32-Target", "password123");
  delay(500);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Define routes BEFORE starting the server
  server.on("/", []() {
    server.send(200, "text/plain", "Hello from ESP32");
  });

  server.on("/scan.txt", []() {
    server.send(200, "text/plain", "SSID,RSSI,Security\nTestNetwork,-45,WPA2\n");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();  // Must be called constantly to process requests
}
