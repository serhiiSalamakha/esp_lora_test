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

// SPI pins for communication with LR1121
#define SPI_PIN_DIO1              14
#define SPI_PIN_BUSY              32
#define SPI_PIN_MISO              27
#define SPI_PIN_MOSI              26
#define SPI_PIN_SCK               25
#define SPI_PIN_CS                33
#define SPI_PIN_RESET             13

// LED TX status (RGB WS2812)
#define LED_TX_STATUS             23

// Radio
#define RADIO_FREQUENCY           868.1
#define RADIO_BANDWIDTH           125.0
#define RADIO_SPREADING_FACTOR    7
#define RADIO_CODING_RATE         8 // LORA_CR_4_8
#define RADIO_OUTPUT_POWER        13

// Lora TDOA commands
#define CMD_TDOA_BEACON           0x10

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CONFIG_H */
