/*
 * config.h - user configuration file
 */

#ifndef CONFIG_H
#define CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// Serial
#define SERIAL_BAUD_RATE          115200

// SX1280 pins
#define GPIO_PIN_DIO1              4
#define GPIO_PIN_BUSY              5
#define GPIO_PIN_MISO              12
#define GPIO_PIN_MOSI              13
#define GPIO_PIN_SCK               14
#define GPIO_PIN_CS                15
#define GPIO_PIN_RESET             16

// Radio
#define RADIO_FREQUENCY           2400.0
#define RADIO_BANDWIDTH           1625
#define RADIO_SPREADING_FACTOR    5
#define RADIO_OUTPUT_POWER        0
#define RADIO_CODING_RATE         5 // LORA_CR_4_5

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CONFIG_H */
