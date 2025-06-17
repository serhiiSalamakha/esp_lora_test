/*
 * main.cpp - main file of master/slave sw ranging app
 */

#include "config.h"

#include <error.h>
#include <Arduino.h>
#include <RadioLib.h>
#include <ranging_sw.h>

#define RANGING_DATA_REPEAT_MS    1000

static SX1280 radio = new Module(GPIO_PIN_CS, GPIO_PIN_DIO1, GPIO_PIN_RESET, GPIO_PIN_BUSY);
static struct ranging_sw_data ranging_sw_data;

volatile bool rx_event_flag = false;

IRAM_ATTR void rx_event_handler(void)
{
    rx_event_flag = true;
}

static uint64_t get_time_func(void)
{
    return static_cast<uint64_t>(micros());
}

static int send(const uint8_t* data, uint16_t len)
{
    int err;

    Serial.printf("xxx sending: %x\n", data[0]);

    err = radio.transmit(data, len);
    if (err != RADIOLIB_ERR_NONE) {
        Serial.printf("TX failed (err = %d)\n", err);
    }

    return err;
}

static int receive(uint8_t* data, uint16_t max_len)
{
    int err;
    String rx_str;

    err = radio.startReceive();
    if (err != RADIOLIB_ERR_NONE) {
        error_event_handler("RX start failed", err);
    }

    while (!rx_event_flag);

    rx_event_flag = false;

    err = radio.readData(rx_str);
    if (err != RADIOLIB_ERR_NONE) {
        error_event_handler("Receive failed", err);
    }

    return err;
}

void setup()
{
    int err;

    Serial.begin(SERIAL_BAUD_RATE);
#ifdef RANGING_MASTER
    Serial.printf("\nMaster sw ranging app initializing...\n");
#else
    Serial.printf("\nSlave sw ranging app initializing...\n");
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
    radio.setPacketReceivedAction(rx_event_handler);

    ranging_sw_init(&ranging_sw_data, get_time_func, send, receive);

    Serial.printf("Radio initialized\n");
}

void loop()
{
#ifdef RANGING_MASTER
    Serial.printf("Master sw ranging...\n");
    Serial.printf("SW ranging result: %fm\n", ranging_sw_master_get_dist(&ranging_sw_data));

    delay(RANGING_DATA_REPEAT_MS);
#else
    static uint32_t rx_cnt = 0;
    int err;

    err = ranging_sw_slave_handle_request(&ranging_sw_data);
    if (!err) {
        Serial.printf("Slave sent ranging response (rx cnt = %u)\n", ++rx_cnt);
    }
#endif /* RANGING_MASTER */
}
