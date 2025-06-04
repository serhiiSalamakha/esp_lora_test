# 📡 ESP8285 + SX1280 LoRa Ranging Firmware

This project provides firmware for a LoRa-based system using an **ESP8285 microcontroller** and an **SX1280 transceiver**

## Build Instructions
```bash
# 1. Clone the repository with submodules:
git clone https://github.com/serhiiSalamakha/esp8285_sx1280_ranging.git
git submodule update --init --recursive

# 2. Install PlatformIO CLI:
pip install platformio

# 3. Install platform support for ESP8266/8285:
platformio platform install espressif8266

# 4. Build the firmware:
# For transmitter:
platformio run -e esp8285_sx1280_tx --target clean
platformio run -e esp8285_sx1280_tx

# For receiver:
platformio run -e esp8285_sx1280_rx --target clean
platformio run -e esp8285_sx1280_rx
