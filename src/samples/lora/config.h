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
#define RADIO_FREQUENCY           868
#define RADIO_BANDWIDTH           406.25
#define RADIO_SPREADING_FACTOR    10
#define RADIO_CODING_RATE         8 // LORA_CR_4_8
#define RADIO_OUTPUT_POWER        13

// Lora commands
#define CMD_TDOA_BEACON           0x10

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CONFIG_H */
