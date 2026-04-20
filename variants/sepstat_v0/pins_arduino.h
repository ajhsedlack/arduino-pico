#pragma once

#include <cyw43_wrappers.h>

// Pin definitions taken from:
//    https://datasheets.raspberrypi.org/pico/pico-datasheet.pdf

#define PICO_RP2350A 1

// LEDs
#define PIN_LED        (64u)

// Serial
//#define PIN_SERIAL1_TX (0u)
//#define PIN_SERIAL1_RX (1u)
//SEPSTAT ONLY
#define PIN_SERIAL1_TX (-1)
#define PIN_SERIAL1_RX (-1)


//#define PIN_SERIAL2_TX (8u)
//#define PIN_SERIAL2_RX (9u)
#define PIN_SERIAL2_TX (-1)
#define PIN_SERIAL2_RX (-1)


// SPI
#define PIN_SPI0_MISO  (16u)
#define PIN_SPI0_MOSI  (19u)
#define PIN_SPI0_SCK   (18u)
#define PIN_SPI0_SS    (17u)

#define PIN_SPI1_MISO  (12u)
#define PIN_SPI1_MOSI  (15u)
#define PIN_SPI1_SCK   (14u)
#define PIN_SPI1_SS    (13u)

// Wire
//#define PIN_WIRE0_SDA  (4u)
//#define PIN_WIRE0_SCL  (5u)
//SEPSTAT ONLY
#define PIN_WIRE0_SDA  (28u)
#define PIN_WIRE0_SCL  (29u)

#define PIN_WIRE1_SDA  (26u)
#define PIN_WIRE1_SCL  (27u)

//#define SERIAL_HOWMANY (3u)
//#define SPI_HOWMANY    (2u)
//#define WIRE_HOWMANY   (2u)

// Real time clock
#define RTC_SDA     (28u)
#define RTC_SCL     (29u)

//multiplexer
// legacy memestat mux
#define MUX_ENABLE  (8u)
#define SWITCH_ENABLE (3u)
// sepstat only
#define MUX_ENABLE2 (12u)


#include "../generic/common.h"
