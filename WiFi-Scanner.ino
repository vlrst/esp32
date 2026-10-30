
#include <WiFi.h>
#include <LittleFS.h>
#include <time.h>

String folderPath;

void setup() {
  Serial.begin(115200);

  if (!LittleFS.begin(true)) {
    Serial.println("[LITTLEFS FAILURE]");
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  time_t now = time(nullptr);
  folderPath = "/ESP_SCAN_" + String((unsigned long)now);

  if (!LittleFS.mkdir(folderPath.c_str())) {
    Serial.println("[COULD NOT CREATE FOLDER]");
    return;
  }

  Serial.println("[SETUP DONE]");
  Serial.println("[WIFI SCANNER STARTED]");
}

  
void loop() {
  // Run five Wi-Fi scans
  for (int j = 0; j < 5; j++) {
    Serial.print("[--------NEW SCAN--------]\r\n");

    String output = "[SCAN: " + String(j + 1) + "]\r\n";
    output += "[SCANNING NETWORKS...]\r\n";

    int n = WiFi.scanNetworks();

    output += "[SCAN COMPLETE]\r\n";
    output += "[NETWORKS FOUND: " + String(n) + "]\r\n";

    for (int i = 0; i < n; i++) {
      output += "[SSID: " + WiFi.SSID(i) + "]\r\n";
      output += "[RSSI: " + String(WiFi.RSSI(i)) + "]\r\n";

      switch (WiFi.encryptionType(i)) {
        case WIFI_AUTH_OPEN:
          output += "[SECURITY: OPEN]\r\n";
          break;

        case WIFI_AUTH_WEP:
          output += "[SECURITY: WEP]\r\n";
          break;

        case WIFI_AUTH_WPA_PSK:
          output += "[SECURITY: WPA]\r\n";
          break;

        case WIFI_AUTH_WPA2_PSK:
          output += "[SECURITY: WPA2]\r\n";
          break;

        case WIFI_AUTH_WPA_WPA2_PSK:
          output += "[SECURITY: WPA/WPA2]\r\n";
          break;

        default:
          output += "[SECURITY: OTHER]\r\n";
          break;
      }

      output += "--------------------------------\r\n";
    }

    Serial.print(output);
    Serial.print("\r\n");

    // Save results
    String filePath = folderPath + "/scan.txt";
    File file = LittleFS.open(filePath.c_str(), FILE_APPEND);

    if (file) {
      file.print(output);
      file.print("\r\n");
      file.close();
      Serial.print("[RESULTS SAVED]\r\n");
    } else {
      Serial.print("[FAILURE TO OPEN FILE]\r\n");
    }

    WiFi.scanDelete();

    delay(2000);
  }

  Serial.print("[ALL 5 SCANS COMPLETE]\r\n");

  // Terminal mode
  while (true) {
    if (Serial.available()) {
      String cmd = Serial.readStringUntil('\n');
      cmd.trim();

      if (cmd == "ls") {
        File root = LittleFS.open("/");
        File file = root.openNextFile();

        Serial.print("[FILESYSTEM CONTENTS]\r\n");

        while (file) {
          Serial.print(file.name());
          Serial.print(" | ");
          Serial.print(file.size());
          Serial.print(" bytes\r\n");

          file = root.openNextFile();
        }

        root.close();
      }

      else if (cmd == "memory") {
        Serial.printf("Total: %u bytes\r\n", LittleFS.totalBytes());
        Serial.printf("Used: %u bytes\r\n", LittleFS.usedBytes());
        Serial.printf("Free: %u bytes\r\n",
          LittleFS.totalBytes() - LittleFS.usedBytes());
      }

      else {
        Serial.print("[UNKNOWN COMMAND]\r\n");
      }
    }

    delay(10);
  }
}
