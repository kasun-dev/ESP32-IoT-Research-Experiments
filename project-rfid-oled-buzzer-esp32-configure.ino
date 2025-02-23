#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Display Settings
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);

// RFID RC522 Settings (Use GPIO numbers instead of 'D' prefix)
#define SS_PIN 5      // SDA (CS) pin
#define RST_PIN 4     // Reset pin
MFRC522 mfrc522(SS_PIN, RST_PIN);

// LED Pin (using GPIO 2, change if using a different pin)
#define LED_PIN 2

// Buzzer Pin (connected to GPIO 15, change if using another pin)
#define BUZZER_PIN 15

void setup() {
  Serial.begin(115200);

  // Initialize OLED Display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }
  // Initialize SPI and RFID
  SPI.begin();
  mfrc522.PCD_Init();

  // Initialize LED and Buzzer Pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Ensure the LED is off initially
  digitalWrite(BUZZER_PIN, LOW);  // Ensure the buzzer is off initially
}

void loop() {
  // Showing ready text
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Tap Here.!");
  display.display();
  display.clearDisplay();

  // Check for a new RFID card
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("Card UID: ");
  display.setTextSize(1);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("Card UID:");

  // Read and print UID
  String uidStr = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidStr += String(mfrc522.uid.uidByte[i], HEX) + " ";
  }

  Serial.println(uidStr);
  display.setTextSize(1);
  display.setCursor(0, 30);
  display.println(uidStr);
  display.display();

  // Turn on the LED for 500ms
  digitalWrite(LED_PIN, HIGH);  // Turn the LED on
  tone(BUZZER_PIN, 500, 200); // Emit beep for 200ms at 1000Hz
  delay(500);                   // Wait for 500ms
  digitalWrite(LED_PIN, LOW);   // Turn the LED off

  delay(2000);  // Delay before the next scan
}
