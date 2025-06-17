/*
 * main_tx.cpp - main file of tx app
 */

#include "config.h"

#include <error.h>
#include <Arduino.h>
#include <RadioLib.h>

#define TX_BUFF_LEN          128
#define TX_DATA_REPEAT_MS    1000

/* Macros to pack the different types */
#define PACK_FIELD(buff, field)           \
do {                                      \
	memcpy(buff, &field, sizeof(field));  \
	buff += sizeof(field);                \
} while (0)

struct lora_tdoa_msg {
    uint8_t cmd;
    uint32_t id;
    uint16_t cnt;
};

SX1280 radio = new Module(GPIO_PIN_CS, GPIO_PIN_DIO1, GPIO_PIN_RESET, GPIO_PIN_BUSY);

static size_t pack_lora_tdoa_msg(struct lora_tdoa_msg *unpack_data,
    uint8_t *pack_data, uint16_t max_pack_data_len)
{
    size_t pack_data_len = sizeof(unpack_data->cmd) + sizeof(unpack_data->id) +
        sizeof(unpack_data->cnt);

    if (pack_data != NULL && pack_data_len <= max_pack_data_len) {
        PACK_FIELD(pack_data, unpack_data->cmd);
        PACK_FIELD(pack_data, unpack_data->id);
        PACK_FIELD(pack_data, unpack_data->cnt);
    } else {
        pack_data_len = 0;
    }

    return pack_data_len;
}

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
    radio.setCodingRate(RADIO_CODING_RATE, false);
    radio.setOutputPower(RADIO_OUTPUT_POWER);

    Serial.printf("Radio initialized\n");
}

void loop()
{
    static uint16_t tx_cnt = 0;
    uint8_t tx_buff[TX_BUFF_LEN];
    struct lora_tdoa_msg tdoa_msg = {
        .cmd = CMD_TDOA_BEACON,
        .id = ESP.getChipId(),
        .cnt = ++tx_cnt,
    };

    Serial.printf("Lora TDoA msg: id=%x, id=%u, cnt=%u\n", tdoa_msg.cmd, tdoa_msg.id, tdoa_msg.cnt);

    do {
        size_t tx_len = pack_lora_tdoa_msg(&tdoa_msg, tx_buff, sizeof(tx_buff));
        if (!tx_len) {
            Serial.printf("Pack lora_tdoa_msg failed\n");
            break;
        }

        int err = radio.transmit(tx_buff, tx_len);
        if (err != RADIOLIB_ERR_NONE) {
            Serial.printf("TX failed (err = %d)\n", err);
        }
    } while(0);

    delay(TX_DATA_REPEAT_MS);
}
