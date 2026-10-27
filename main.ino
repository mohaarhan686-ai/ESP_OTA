#include <WiFi.h>
#include <ArduinoOTA.h>
const char* ssid = "Vimal";
const char* password = "1234567890";
void setup()
 {
  Serial.begin(115200);
  pinMode(2,OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }
  ArduinoOTA.setHostname("ESP32-OTA");
  ArduinoOTA.setPassword("1234567890");
  ArduinoOTA.begin();
  Serial.println("Getting Started with OTA");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop()
 {
  ArduinoOTA.handle();
  Serial.println("Hello Esp");
  delay(1000);
}
