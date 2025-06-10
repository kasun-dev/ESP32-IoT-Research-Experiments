#include <WiFi.h>
#include <WiFiManager.h>
#include <Firebase_ESP_Client.h>

// Provide the token generation process info
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Firebase credentials
#define DATABASE_URL "https://smarthomepro-a1e57-default-rtdb.asia-southeast1.firebasedatabase.app"
#define API_KEY "AIzaSyC3ryhdX2nxYsRPRFgpzDaqzvCbx2JKPNg"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Relay pins
#define RELAY1_PIN 4
#define RELAY2_PIN 5

// Relay states
bool relay1State = false;
bool relay2State = false;

// Function to update relay based on value
void updateRelayState(const String& path, const String& value) {
  bool state = (value == "ON");

  if (path == "/relay1") {
    relay1State = state;
    digitalWrite(RELAY1_PIN, state ? LOW : HIGH);  // ACTIVE-LOW LOGIC
    Serial.println("Relay 1 updated via stream.");
  } else if (path == "/relay2") {
    relay2State = state;
    digitalWrite(RELAY2_PIN, state ? LOW : HIGH);  // ACTIVE-LOW LOGIC
    Serial.println("Relay 2 updated via stream.");
  }
}

// Stream callback function
void streamCallback(FirebaseStream data) {
  Serial.printf("Stream path: %s\n", data.streamPath().c_str());
  Serial.printf("Data path: %s\n", data.dataPath().c_str());
  Serial.printf("Data type: %s\n", data.dataType().c_str());
  Serial.printf("Data: %s\n", data.stringData().c_str());

  String path = data.dataPath();     // e.g., "/relay1"
  String value = data.stringData();  // e.g., "ON"

  updateRelayState(path, value);
}

// Stream timeout handler
void streamTimeoutCallback(bool timeout) {
  if (timeout) {
    Serial.println("Firebase stream timed out. Reconnecting...");
  }
}

void setup() {
  Serial.begin(115200);

  // WiFiManager for dynamic Wi-Fi connection
  WiFiManager wm;
  bool res = wm.autoConnect("SmartHomePro", "12345678");
  if (!res) {
    Serial.println("Wi-Fi Failed");
    ESP.restart();
  }

  Serial.println("Wi-Fi connected!");
  Serial.println(WiFi.localIP());

  // Firebase setup
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;

  auth.user.email = "sampleEmail@gmail.com";     // Optional: For authenticated access
  auth.user.password = "emailPassword";

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Set pin modes
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);

  // Set initial states to OFF (HIGH for active-low relay)
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);

  // Notify Firebase device is online
  if (Firebase.RTDB.setString(&fbdo, "/device/status", "online")) {
    Serial.println("Device status set to online.");
  } else {
    Serial.println("Failed to set status:");
    Serial.println(fbdo.errorReason());
  }

  // Start streaming from /device path
  if (!Firebase.RTDB.beginStream(&fbdo, "/device")) {
    Serial.printf("Stream begin error: %s\n", fbdo.errorReason().c_str());
  }

  Firebase.RTDB.setStreamCallback(&fbdo, streamCallback, streamTimeoutCallback);
}

void loop() {
  // Nothing needed unless you want to do other periodic tasks
}
