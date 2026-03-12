/*
 * main_tx.cpp - main file of tx app
 */

#include "config.h"
#include "error.h"

#include <Arduino.h>
#include <RadioLib.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>

#define TX_BUFF_LEN          128
#define TX_DATA_REPEAT_MS    2000

#define LED_STATUS_COUNT     8
#define LED_STATUS_BRIGHT    1

SPIClass spi(VSPI);

#ifdef RF_CHIP_LR1121
Module mode(SPI_PIN_CS, SPI_PIN_DIO1, SPI_PIN_RESET, SPI_PIN_BUSY, spi);
LR1121 radio(&mode);
#elif RF_CHIP_SX1276
Module mode(SPI_PIN_CS, SPI_PIN_DIO0, SPI_PIN_RESET, SPI_PIN_DIO1, spi);
SX1276 radio(&mode);
#endif

Adafruit_NeoPixel stripgrb(LED_STATUS_COUNT, LED_TX_STATUS, NEO_GRB + NEO_KHZ800);

LoRaWANNode node(&radio, &EU868);

uint8_t nwk_key[16] = {
  0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
  0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00
};
uint8_t app_key[16] = {
  0x00, 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99,
  0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11
};

void setup() {
    int err;

    stripgrb.begin();
    stripgrb.setBrightness(LED_STATUS_BRIGHT);
    stripgrb.setPixelColor(0, stripgrb.Color(255, 0, 0)); stripgrb.show();

    Serial.begin(SERIAL_BAUD_RATE);
    Serial.printf("\nTX lorawan initializing...\n");

    spi.begin(SPI_PIN_SCK, SPI_PIN_MISO, SPI_PIN_MOSI,SPI_PIN_CS);

    err = radio.begin();
    if (err != RADIOLIB_ERR_NONE) {
        error_event_handler("Radio init failed", err);
    }

    uint32_t devAddr = (uint32_t)(ESP.getEfuseMac() >> 16);
    Serial.printf("Device address: 0x%x\n", devAddr);

    node.beginABP(devAddr, nwk_key, nwk_key, nwk_key, app_key);
    err = node.activateABP();
    if (err != RADIOLIB_ERR_NONE && err != RADIOLIB_LORAWAN_NEW_SESSION) {
        error_event_handler("LoRaWAN ABP activation failed", err);
    }

#ifdef RF_CHIP_SX1276
    pinMode(MISC_FAN_EN, OUTPUT);
    pinMode(POWER_RX_EN, OUTPUT);
    pinMode(POWER_TX_EN, OUTPUT);

    dacWrite(POWER_APC, 255);

    digitalWrite(MISC_FAN_EN, HIGH);
    digitalWrite(POWER_RX_EN, LOW);
    digitalWrite(POWER_TX_EN, HIGH);
#endif

    stripgrb.setPixelColor(0, stripgrb.Color(0, 255, 0)); stripgrb.show();
}

void loop() {
    int err;
    static bool led_status_on = true;
    static uint32_t cnt;

    do {
        err = node.setDatarate(DR3_SF9_BW125Hz);
        err |= node.setTxPower(TX_POWER_VALUE);
        if (err != RADIOLIB_ERR_NONE) {
            stripgrb.setPixelColor(0, stripgrb.Color(255, 0, 0)); stripgrb.show();
            Serial.printf("Set configuration failed (err = %d)\n", err);
            break;
        }

        err = node.sendReceive(NULL, 0, 1, false);
        if (err != RADIOLIB_ERR_NONE && err != RADIOLIB_ERR_TX_TIMEOUT) {
            stripgrb.setPixelColor(0, stripgrb.Color(255, 0, 0)); stripgrb.show();
            Serial.printf("TX failed (err = %d)\n", err);
            break;
        }

        Serial.printf("TX: cnt = %d\n", ++cnt);

        if (led_status_on) {
            stripgrb.setPixelColor(0, stripgrb.Color(0, 0, 0)); stripgrb.show();
            led_status_on = false;
        } else {
            stripgrb.setPixelColor(0, stripgrb.Color(0, 255, 0)); stripgrb.show();
            led_status_on = true;
        }
    } while(0);

    delay(TX_DATA_REPEAT_MS);
}
