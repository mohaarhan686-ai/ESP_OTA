# ESP32 OTA (Over-The-Air) Programming

Learn how to upload new firmware to an ESP32 over Wi-Fi, without connecting the USB cable every time.

This repository is designed for beginners. The explanation follows the same order as the code, so you can understand the logic and later recall the program without memorising every line.

## What is OTA?

**OTA means Over-The-Air.** It lets you upload a new program to your ESP32 through a Wi-Fi network after the first USB upload has configured OTA.

Typical workflow:

1. Connect ESP32 to Wi-Fi.
2. Configure the OTA device name and upload password.
3. Start the OTA service.
4. Keep calling `ArduinoOTA.handle()` inside `loop()`.
5. Select the ESP32 network port in Arduino IDE and upload the next sketch wirelessly.

![ESP32 OTA workflow](images/ota-workflow.png)

## Hardware and software

- ESP32 development board
- USB data cable (needed for the first upload and troubleshooting)
- Wi-Fi network accessible by both the computer and ESP32
- Arduino IDE
- ESP32 board support package installed in Arduino IDE
- `ArduinoOTA` library, normally included with the ESP32 Arduino core

## Files in this repository

- `ESP32_OTA.ino` — commented Arduino sketch
- `images/ota-workflow.png` — OTA workflow diagram
- `images/esp32-ota-cover.png` — repository cover illustration

## Before you upload

Open `ESP32_OTA.ino` and replace these placeholders with your own credentials:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
const char* otaPassword = "YOUR_OTA_PASSWORD";
```

Use a Wi-Fi network that your computer and ESP32 can both access. Most ESP32 boards support 2.4 GHz Wi-Fi, so use a compatible network.

**Security:** Do not commit real Wi-Fi credentials or OTA passwords to a public GitHub repository. For a public project, use placeholders in the code and enter your credentials locally before uploading.

## Code explained in the same flow

### 1. Include libraries

```cpp
#include <WiFi.h>
#include <ArduinoOTA.h>
```

- `WiFi.h` provides Wi-Fi connection functions.
- `ArduinoOTA.h` provides OTA update functions.

### 2. Store Wi-Fi credentials

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```

`ssid` is the Wi-Fi network name; `password` is its password. These are used by `WiFi.begin()`.

### 3. Initialise the board in `setup()`

```cpp
Serial.begin(115200);
pinMode(2, OUTPUT);
```

`setup()` runs once after startup or reset. Serial Monitor helps us see status messages. GPIO 2 is configured as an output; the sketch toggles it as a simple activity indicator. On some ESP32 boards, the onboard LED is not connected to GPIO 2—change the pin if needed.

### 4. Connect to Wi-Fi

```cpp
WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
  delay(500);
}
```

`WiFi.begin()` starts connecting. The `while` loop checks the status every 500 ms and waits until the board is connected. This simple version waits indefinitely if the network is unavailable; the sketch below includes a timeout and reports a failure.

### 5. Configure and start OTA

```cpp
ArduinoOTA.setHostname("ESP32-OTA");
ArduinoOTA.setPassword(otaPassword);
ArduinoOTA.begin();
```

- `setHostname()` sets the network name shown for the OTA device.
- `setPassword()` sets the password used for OTA uploads.
- `begin()` starts the OTA service.

The sketch also prints the local IP address using `WiFi.localIP()`.

### 6. Keep OTA alive in `loop()`

```cpp
ArduinoOTA.handle();
```

Call this repeatedly in `loop()` so the ESP32 can process OTA upload requests. The example also prints a message every second and toggles GPIO 2.

![OTA upload flow](images/ota-upload-flow.png)

## Uploading the sketch

### First upload (USB)

1. Open `ESP32_OTA.ino` in Arduino IDE.
2. Select your ESP32 board and the USB/COM port.
3. Enter your Wi-Fi name and password, plus a strong OTA password.
4. Upload the sketch using USB.
5. Open Serial Monitor at **115200 baud**.
6. Wait for `OTA Ready` and note the IP address printed.

### Later uploads (Wi-Fi)

1. Keep the ESP32 powered on and connected to the same network.
2. In Arduino IDE, open **Tools → Port**.
3. Select the network port named `ESP32-OTA` (the exact menu wording can vary by IDE/core version).
4. Upload your new sketch. Enter the OTA password if prompted.

If the network port does not appear, check that the computer and ESP32 are on the same network, local network discovery is enabled, and your firewall is not blocking discovery. You can also use the printed IP address to help diagnose the connection.

## Common problems

| Problem | What to check |
|---|---|
| ESP32 never connects | Wi-Fi name/password, 2.4 GHz compatibility, signal strength |
| OTA port is missing | Same network, firewall, network discovery, ESP32 still powered on |
| OTA password rejected | Match the password configured in the sketch |
| LED does not blink | Your board may use a different LED GPIO, or have no onboard LED |
| Upload fails midway | Keep power stable, reduce network interference, retry over USB if needed |

## Learning checklist

- [ ] Explain what `WiFi.begin()` does.
- [ ] Explain why `while (WiFi.status() != WL_CONNECTED)` is used.
- [ ] Recall the order: hostname → OTA password → `ArduinoOTA.begin()`.
- [ ] Explain why `ArduinoOTA.handle()` belongs in `loop()`.
- [ ] Upload once through USB, then try an OTA upload.

## Important note

OTA is useful for development, but it is not a substitute for safe recovery access. Keep a USB cable available in case Wi-Fi or OTA configuration fails.
