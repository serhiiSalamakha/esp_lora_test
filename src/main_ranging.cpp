/*
 * main_ranging.cpp - main file of master/slave ranging app
 */

#include <config.h>
#include <error.h>
#include <Arduino.h>
#include <RadioLib.h>

#define RANGING_SLAVE_ADDR        0x12345678
#define RANGING_DATA_REPEAT_MS    1000

SX1280 radio = new Module(GPIO_PIN_CS, GPIO_PIN_DIO1, GPIO_PIN_RESET, GPIO_PIN_BUSY);

void setup()
{
    int err;

    Serial.begin(SERIAL_BAUD_RATE);
#ifdef RANGING_MASTER
    Serial.printf("\nMaster ranging app initializing...\n");
#else
    Serial.printf("\nSlave ranging app initializing...\n");
#endif /* RANGING_MASTER */

    err = radio.begin();
    if (err != RADIOLIB_ERR_NONE) {
        error_event_handler("Radio init failed", err);
    }

    radio.setFrequency(RADIO_FREQUENCY);
    radio.setBandwidth(RADIO_BANDWIDTH);
    radio.setSpreadingFactor(RADIO_SPREADING_FACTOR);
    radio.setOutputPower(RADIO_OUTPUT_POWER);
    radio.setCodingRate(RADIO_CODING_RATE, false);

    Serial.printf("Radio initialized\n");
}

void loop()
{
    int err;

#ifdef RANGING_MASTER
    Serial.printf("Master ranging...\n");
    err = radio.range(true, RANGING_SLAVE_ADDR);

    if (err == RADIOLIB_ERR_NONE) {
        Serial.printf("Ranging tx result: %fm\n", radio.getRangingResult());
    } else if (err == RADIOLIB_ERR_RANGING_TIMEOUT) {
        Serial.printf("Ranging tx timeout\n");
    } else {
        Serial.printf("Ranging tx failed (err = %d) \n", err);
    }

    delay(RANGING_DATA_REPEAT_MS);
#else
    static uint32_t ranging_cnt = 0;

    Serial.printf("Slave ranging...\n");
    err = radio.range(false, RANGING_SLAVE_ADDR);
    if (err == RADIOLIB_ERR_NONE) {
        Serial.printf("Ranging done: cnt = %u\n", ++ranging_cnt);
    } else if (err == RADIOLIB_ERR_RANGING_TIMEOUT) {
        Serial.printf("Ranging rx timeout\n");
    } else {
        Serial.printf("Ranging rx failed (err = %d) \n", err);
    }
#endif /* RANGING_MASTER */
}
