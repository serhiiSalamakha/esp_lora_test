/*
 * main_tx.cpp - main file of tx app
 */

#include <config.h>
#include <error.h>
#include <Arduino.h>
#include <RadioLib.h>

#define TX_BUFF_LEN          128
#define TX_DATA_REPEAT_MS    1000

SX1280 radio = new Module(GPIO_PIN_CS, GPIO_PIN_DIO1, GPIO_PIN_RESET, GPIO_PIN_BUSY);

void setup()
{
    int err;

    Serial.begin(SERIAL_BAUD_RATE);
    Serial.printf("\nTX initializing...\n");

    err = radio.begin();
    if (err != RADIOLIB_ERR_NONE) {
        error_event_handler("Radio init failed", err);
    }

    radio.setFrequency(RADIO_FREQUENCY);
    radio.setBandwidth(RADIO_BANDWIDTH);
    radio.setSpreadingFactor(RADIO_SPREADING_FACTOR);
    radio.setOutputPower(RADIO_OUTPUT_POWER);

    Serial.printf("Radio initialized\n");
}

void loop()
{
    static int tx_cnt = 0;
    int err;
    char tx_str[TX_BUFF_LEN];

    snprintf(tx_str, sizeof(tx_str), "sx1280 msg %d", ++tx_cnt);

    Serial.printf("Sending: %s\n", tx_str);

    err = radio.transmit(tx_str);
    if (err != RADIOLIB_ERR_NONE) {
        Serial.printf("TX failed (err = %d)\n", err);
    }

    delay(TX_DATA_REPEAT_MS);
}
