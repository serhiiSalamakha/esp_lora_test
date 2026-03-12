# 📡 **ESP8285 + SX1280** and **ESP32 + LR1121/SX1276** LoRa TX/RX applications

This project provides firmware for a LoRa-based system using the **ESP8285 + SX1280** and **ESP32 + LR1121/SX1276**

## Build Instructions
```bash
# 1. Clone the repository with submodules:
git clone https://github.com/serhiiSalamakha/esp_lora_test.git
git submodule update --init --recursive
git -C lib/RadioLib checkout b0a3f2d4f9dee58eb34525a34a6dd0fb92c4d384

# 2. Install PlatformIO CLI:
pip install platformio

# 3. Install platform support for ESP8266/8285:
platformio platform install espressif8266

# 4. Build the firmware:
# For transmitter:
platformio run -e esp8285_sx1280_lora_tx
platformio run -e esp32_lr1121_lora_wan_tx
platformio run -e esp32_sx1276_lora_wan_tx

# For receiver:
platformio run -e esp8285_sx1280_lora_rx
