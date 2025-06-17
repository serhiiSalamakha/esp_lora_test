/*
 * main_rx.cpp - main file of rx app
 */

#include "config.h"

#include <error.h>
#include <Arduino.h>
#include <RadioLib.h>

#define RX_BUFF_LEN          128

/* Macros for unpack the different types */
#define UNPACK_FIELD(buff, field)         \
do {                                      \
	memcpy(&field, buff, sizeof(field));  \
	buff += sizeof(field);                \
} while (0)

struct lora_tdoa_msg {
    uint8_t cmd;
    uint32_t id;
    uint16_t cnt;
};

SX1280 radio = new Module(GPIO_PIN_CS, GPIO_PIN_DIO1, GPIO_PIN_RESET, GPIO_PIN_BUSY);

volatile bool rx_event_flag = false;

IRAM_ATTR void rx_event_handler(void)
{
  rx_event_flag = true;
}

static void unpack_lora_tdoa_msg(struct lora_tdoa_msg *unpack_data,
    uint8_t *pack_data, uint16_t pack_data_len)
{
    size_t expect_len = sizeof(unpack_data->cmd) + sizeof(unpack_data->id) +
        sizeof(unpack_data->cnt);

    if (pack_data != NULL && expect_len <= pack_data_len) {
        UNPACK_FIELD(pack_data, unpack_data->cmd);
        UNPACK_FIELD(pack_data, unpack_data->id);
        UNPACK_FIELD(pack_data, unpack_data->cnt);
    }
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
    radio.setCodingRate(RADIO_CODING_RATE, false);
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

        rx_event_flag = false;

        do {
            uint8_t rx_buff[RX_BUFF_LEN];
            struct lora_tdoa_msg tdoa_msg;

            size_t rx_len = radio.getPacketLength(false);

            err = radio.readData(rx_buff, sizeof(rx_buff));
            if (err != RADIOLIB_ERR_NONE) {
                Serial.printf("Receive failed (err = %d) \n", err);
                break;
            }

            unpack_lora_tdoa_msg(&tdoa_msg, rx_buff, rx_len);

            Serial.printf("Received(rssi=%.2fdBm, snr=%.2fdB, freq_offset=%.2fHz): "
                "id=%x, id=%u, cnt=%u\n",
                radio.getRSSI(), radio.getSNR(), radio.getFrequencyError(),
                tdoa_msg.cmd, tdoa_msg.id, tdoa_msg.cnt);
        } while(0);

        err = radio.startReceive();
        if (err != RADIOLIB_ERR_NONE) {
            error_event_handler("RX re-start failed", err);
        }
    }
}
