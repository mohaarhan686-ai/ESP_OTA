/*
  Project: ESP32 OTA (Over-The-Air) Programming
  Purpose: Upload new firmware to an ESP32 over Wi-Fi after the first USB upload.

  Before uploading:
  1. Replace the Wi-Fi name and password below.
  2. Set a strong OTA password.
  3. Do not publish real credentials in a public GitHub repository.
*/

#include <WiFi.h>       // Connect the ESP32 to a Wi-Fi network.
#include <ArduinoOTA.h> // Receive firmware updates over Wi-Fi.

// ---------- 1. Wi-Fi credentials ----------
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// Password requested for OTA firmware uploads.
// Keep this different from your Wi-Fi password.
const char* otaPassword = "YOUR_OTA_PASSWORD";

// ---------- 2. Setup: runs once after startup/reset ----------
void setup() {
  Serial.begin(115200); // Start Serial Monitor communication.
  pinMode(2, OUTPUT);   // GPIO 2 as output; onboard LED pin varies by board.
  digitalWrite(2, LOW); // Start with this output LOW.

  // ---------- 3. Start Wi-Fi connection ----------
  WiFi.mode(WIFI_STA); // Connect to an existing Wi-Fi network (station mode).
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");

  // Wait for connection, but do not wait forever if credentials/network are wrong.
  const unsigned long startTime = millis();
  const unsigned long wifiTimeout = 20000; // 20 seconds

  while (WiFi.status() != WL_CONNECTED &&
         millis() - startTime < wifiTimeout) {
    delay(500); // Wait half a second before checking again.
    Serial.print(".");
  }

  // If Wi-Fi failed, stop setup here. OTA cannot work without network access.
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nWi-Fi connection failed. Check credentials/network.");
    return;
  }

  Serial.println("\nWi-Fi connected!");

  // ---------- 4. Configure OTA identity and password ----------
  ArduinoOTA.setHostname("ESP32-OTA"); // Name shown for this OTA device.
  ArduinoOTA.setPassword(otaPassword); // Password for OTA uploads.

  // Optional callbacks: useful messages while an update is happening.
  ArduinoOTA.onStart([]() {
    Serial.println("OTA update started");
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA update finished");
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("OTA error: %u\n", error);
  });

  // ---------- 5. Start the OTA service ----------
  ArduinoOTA.begin();

  Serial.println("OTA Ready");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print ESP32's local network IP address.
}

// ---------- 6. Loop: runs repeatedly while the board is on ----------
void loop() {
  // Must be called repeatedly so OTA requests can be processed.
  if (WiFi.status() == WL_CONNECTED) {
    ArduinoOTA.handle();
  }

  // Print a status message and toggle GPIO 2 about once per second.
  static unsigned long lastPrint = 0;

  if (millis() - lastPrint >= 1000) {
    lastPrint = millis();
    Serial.println("Hello ESP32");

    // This is only a visual activity indicator if an LED is connected to GPIO 2.
    digitalWrite(2, !digitalRead(2));
  }
}
