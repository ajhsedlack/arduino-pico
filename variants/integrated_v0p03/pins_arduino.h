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

/* SERIAL — NOT USED BY THIS BOARD. The console is USB CDC; V0p03 has no UART
 * header. cores/rp2040/SerialUART.cpp references PIN_SERIAL1_* and PIN_SERIAL2_*
 * unconditionally, so they must exist to link.
 *
 * Pointed at free pins (44-47) for the same reason as SPI0 below: every low GPIO
 * on this board carries a live signal, and aliasing an unused peripheral onto one
 * turns an accidental Serial1.begin() into a conflict on the DAC or the mux.
 * ⚠ These are PLACEHOLDERS, not a validated UART mapping — if a UART is ever
 * wanted here, pick the pins from the RP2350B FUNCSEL table rather than trusting
 * these. */
#define PIN_SERIAL1_TX      (44u)
#define PIN_SERIAL1_RX      (45u)
#define PIN_SERIAL2_TX      (46u)
#define PIN_SERIAL2_RX      (47u)

/* SPI0 — NOT USED BY THIS BOARD, but variants/generic/common.h references
 * PIN_SPI0_MISO/SCK/SS/MOSI unconditionally (`static const uint8_t MISO = ...`),
 * so the group has to exist or the variant will not compile.
 *
 * Pointed at GPIO40-43 deliberately: that is a REAL RP2350B SPI0 function group
 * (the SPI pin roles repeat every 8 GPIOs — 40=RX, 41=CSn, 42=SCK, 43=TX), and
 * those pins are unused on V0p03. Do NOT repoint these at pins the board already
 * drives: GPIO0 PSRAM_CS, 1 RTC ~INT, 2/3 I2C0, 6-19 R-2R DAC, 20 RTC CLKOUT,
 * 21-24 AD5940 GPIO2..5, 25 LED, 26 AD5940 INT, 27-30 SPI1/AD5940, 31 /AD_RST,
 * 32-39 mux. Aliasing SPI0 onto any of those turns an accidental SPI.begin()
 * into a pin conflict on a live signal.
 *
 * ⚠ Whether GPIO40-43 are broken out on the PCB is irrelevant here — nothing on
 * this board begins SPI0. The AD5940 runs bit-bang today and would use SPI1 if
 * hardware SPI is enabled later (handoff §4). */
#define PIN_SPI0_MISO       (40u)
#define PIN_SPI0_SS         (41u)
#define PIN_SPI0_SCK        (42u)
#define PIN_SPI0_MOSI       (43u)

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
 * `TwoWire Wire1(...)` instantiation on PIN_WIRE1_* but its IRQ trampoline
 * `_handler1()` references Wire1 unconditionally, so omitting the group gives
 * `undefined reference to Wire1` at link time rather than a clear diagnostic.
 * GPIO4/5 are free on V0p03 and are a valid I2C0 SDA/SCL pair. */
#define PIN_WIRE1_SDA       (4u)
#define PIN_WIRE1_SCL       (5u)

#include "../generic/common.h"
