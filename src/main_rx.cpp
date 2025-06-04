/*
 * main.cpp - main rx application file
 */

#include <config.h>
#include <error.h>
#include <Arduino.h>
#include <RadioLib.h>

SX1280 radio = new Module(GPIO_PIN_CS, GPIO_PIN_DIO1, GPIO_PIN_RESET, GPIO_PIN_BUSY);

volatile bool rx_event_flag = false;

IRAM_ATTR void rx_event_handler(void)
{
  rx_event_flag = true;
}

void setup()
{
    int err;

    Serial.begin(SERIAL_BAUD_RATE);
    Serial.printf("\nRX initializing...\n");

    err = radio.begin();
    if (err != RADIOLIB_ERR_NONE) {
        error_event_handler("Radio init failed", err);
    }

    radio.setFrequency(RADIO_FREQUENCY);
    radio.setBandwidth(RADIO_BANDWIDTH);
    radio.setSpreadingFactor(RADIO_SPREADING_FACTOR);
    radio.setOutputPower(RADIO_OUTPUT_POWER);
    radio.setPacketReceivedAction(rx_event_handler);

    Serial.printf("Radio initialized\n");

    err = radio.startReceive();
    if (err != RADIOLIB_ERR_NONE) {
        error_event_handler("RX start failed", err);
    }

    Serial.printf("Listening...\n");
}

void loop() {
  if (rx_event_flag) {
    int err;
    String rx_str;

    rx_event_flag = false;

    err = radio.readData(rx_str);
    if (err == RADIOLIB_ERR_NONE) {
        Serial.printf("Received(rssi=%.2fdBm, snr=%.2fdB, freq=%.2fHz): %s\n",
            radio.getRSSI(), radio.getSNR(), radio.getFrequencyError(), rx_str.c_str());
    } else {
        Serial.printf("Receive failed (err = %d) \n", err);
    }

    err = radio.startReceive();
    if (err != RADIOLIB_ERR_NONE) {
        error_event_handler("RX re-start failed", err);
    }
  }
}
