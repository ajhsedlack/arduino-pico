#pragma once

#include <cyw43_wrappers.h>

/*
 * integrated_pstat V0p03 — RP2350B (QFN-80, 48 GPIO) + CYW43 radio.
 *
 * Pin map extracted from integrated_pstat.kicad_pcb; the reasoning behind every
 * choice is in agent-plans:
 *   memestat/ad5940-driver/integrated-pstat-v0p03-firmware-variant-2026-09-01.md
 *   memestat/ad5940-driver/integrated-pstat-v0p03-variant-handoff-2026-09-01.md
 *
 * Modelled on variants/memestat_v0 for structure and on
 * variants/waveshare_rp2350b_plus_w for the RP2350B specifics.
 */

/* RP2350B — this is the whole mechanism. variants/generic/common.h:193 branches
 * on `defined(PICO_RP2350) && !PICO_RP2350A` to expose D30..D47, which this
 * board needs: the mux sits on GPIO32-39. */
#define PICO_RP2350A 0
#define PINS_COUNT          (48u)
#define NUM_DIGITAL_PINS    (48u)

/* LED. ⚠ DELIBERATELY THE REAL PIN, not the (64u) placeholder memestat_v0 uses.
 * That variant defines PIN_LED (64u) while board_pins.h redefines it to the real
 * pin, so LED_BUILTIN resolves BY INCLUDE ORDER -- the live bug recorded in
 * agent-plans pure-pico-migration-2026-08-26.md §2.1. Here the two agree: 25.
 * Keep them in sync; board_pins.h INTEGRATED_V0P03 must say 25 as well. */
#define PIN_LED             (25u)

/* PSRAM chip select — /CS_PSRAM via R2 0R. */
#define RP2350_PSRAM_CS     (0u)

/* SERIAL — NOT USED BY THIS BOARD. The console is USB CDC (`Serial`), which is
 * independent of these; V0p03 has no UART header. cores/rp2040/SerialUART.cpp
 * instantiates Serial1/Serial2 unconditionally (:542/:548) and its IRQ handler
 * references them, so PIN_SERIAL1_* / PIN_SERIAL2_* must exist to link.
 *
 * ⚠ THEY MUST NOT NAME A REAL PIN. There is no free GPIO on this board -- all 48
 * carry a net -- so any "spare" pin picked here is a live one. This previously
 * read 44/45/46/47, which are WRL_ON / WRL_CLK / WRL_DAT / WRL_~CS: an
 * accidental Serial1.begin() would have driven the CYW43 radio.
 *
 * 0xff is NOPIN. Spelled as the literal because Arduino.h includes this file at
 * :32 and does not define NOPIN until :174. sepstat_v0 does the same thing with
 * (-1); the value here is explicit so it cannot depend on pin_size_t's width. */
#define PIN_SERIAL1_TX      (0xffu)   /* NOPIN */
#define PIN_SERIAL1_RX      (0xffu)   /* NOPIN */
#define PIN_SERIAL2_TX      (0xffu)   /* NOPIN */
#define PIN_SERIAL2_RX      (0xffu)   /* NOPIN */

/* SPI0 — DELIBERATELY NOT DEFINED. This board has no SPI0: the AD5940 is on
 * SPI1 and nothing else uses the peripheral. SPI.cpp guards the SPI object on
 * PIN_SPI0_MISO, and variants/generic/common.h now guards its SS/MOSI/MISO/SCK
 * aliases on PIN_SPI0_SS, so omitting the group is legal and leaves no object
 * that could be begun by accident.
 *
 * ⚠ Do not "helpfully" add it back. It previously read GPIO40-43, which are
 * D40-D43 on J8 pads 5/6/8/9 -- mux control outputs driven by this board. An
 * accidental SPI.begin() would have driven them. There is no free GPIO here to
 * move it to; the correct answer is to have no SPI0 at all. */

/* SPI1 defaults to the AD5940 pins, so the optional hardware-SPI switch (handoff
 * §4) needs no further pin plumbing. The board currently runs the BIT-BANG path
 * (~450 kHz) because ad5940_port.h only auto-enables AD5940_SPI_HW for
 * MEMESTAT_V0/SEPSTAT_V0 — correct and pin-agnostic; bring up on bit-bang first. */
#define PIN_SPI1_MISO       (28u)
#define PIN_SPI1_MOSI       (27u)
#define PIN_SPI1_SCK        (30u)
#define PIN_SPI1_SS         (29u)

/* I2C0 — PCF85063A RTC.
 * ⚠ Do NOT define RTC_SDA / RTC_SCL here. The RTC implementations do
 * `#ifdef RTC_SDA` and SILENTLY REPORT THE RTC ABSENT if it is missing, so the
 * pin source must be board_pins.h, not this fork
 * (pure-pico-migration-2026-08-26.md §2.4). */
#define PIN_WIRE0_SDA       (2u)
#define PIN_WIRE0_SCL       (3u)

/* I2C1 — NOT USED. Required to LINK, not merely to compile: Wire.cpp guards the
 * `TwoWire Wire1(...)` instantiation on PIN_WIRE1_*, but its IRQ trampoline
 * `_handler1()` references Wire1 unconditionally, so omitting the group gives
 * `undefined reference to Wire1` at link time rather than a clear diagnostic.
 *
 * ⚠ NOPIN, not "a free pair". This previously read GPIO4/5, which are D4/D5 on
 * J8 pads 2/3 -- mux control outputs driven by this board, not spare pins. */
#define PIN_WIRE1_SDA       (0xffu)   /* NOPIN */
#define PIN_WIRE1_SCL       (0xffu)   /* NOPIN */

#include "../generic/common.h"
