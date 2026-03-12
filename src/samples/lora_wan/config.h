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

#ifdef RF_CHIP_LR1121
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

#elif RF_CHIP_SX1276
// SPI pins for communication with SX1276
#define SPI_PIN_DIO0              36
#define SPI_PIN_DIO1              37
#define SPI_PIN_MISO              33
#define SPI_PIN_MOSI              32
#define SPI_PIN_SCK               25
#define SPI_PIN_CS                27
#define SPI_PIN_RESET             2

// LED TX status (RGB WS2812)
#define LED_TX_STATUS             22

// RF
#define MISC_FAN_EN               4
#define POWER_RX_EN               10
#define POWER_TX_EN               14
#define POWER_APC                 26
#endif

// Data Rate
#define DR0_SF12_BW125Hz          0
#define DR1_SF11_BW125Hz          1
#define DR2_SF10_BW125Hz          2
#define DR3_SF9_BW125Hz           3
#define DR4_SF8_BW125Hz           4
#define DR5_SF7_BW125Hz           5

// TX power
#define TX_POWER_VALUE            16

// Lora TDOA commands
#define CMD_TDOA_BEACON           0x10

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CONFIG_H */
