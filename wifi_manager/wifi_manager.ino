#include <WiFi.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

void setup() {
  Serial.begin(115200);

  // Create the WiFiManager object
  WiFiManager wm;

  // Uncomment this line to reset saved WiFi credentials on each boot (for testing)
  // wm.resetSettings();

  // Automatically start the config portal if WiFi is not connected
  bool res;
  res = wm.autoConnect("ESP32_ConfigAP", "12345678"); // AP name and password

  if (!res) {
    Serial.println("Failed to connect");
    // Optionally, restart ESP32 or go into deep sleep
    ESP.restart();
  } else {
    Serial.println("Connected to Wi-Fi!");
    Serial.println(WiFi.localIP());
  }
}

void loop() {
  // Your main code (e.g., Firebase communication)
}
