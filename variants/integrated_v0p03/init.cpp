/*
    Initialize the Pico W WiFi driver

    Copyright (c) 2022 Earle F. Philhower, III <earlephilhower@yahoo.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <cyw43_wrappers.h>

extern "C" void initVariant() {
    /*  V0p03 puts the RM2 above GPIO31, so the defaults do not apply and neither
        do memestat's. Taken from integrated_pstat.kicad_pcb, U7 (RP2350_80QFN):

            pad 55 = GPIO44 -> /WRL_ON
            pad 56 = GPIO45 -> /WRL_CLK
            pad 57 = GPIO46 -> /WRL_DAT
            pad 58 = GPIO47 -> /WRL_~{CS}

        DATA_OUT / DATA_IN / HOST_WAKE are all the single half-duplex DAT line,
        the same way memestat collapses its three onto one pin.

        ⚠ This file was previously a byte-identical copy of variants/memestat_v0,
        so it programmed 24/25/26/27. On THIS board those are AD5940 GPIO2 (R87),
        the LED (R16), AD5940 INT (R89) and /MOSI -- i.e. it would have driven the
        AFE's interrupt line and its SPI bus while leaving the real radio dark.
        If you copy this variant again, this array is the first thing to change.
    */
    static uint cyw43_pin_array[CYW43_PIN_INDEX_WL_COUNT] = {
        44, // CYW43_PIN_INDEX_WL_REG_ON    -> /WRL_ON
        46, // CYW43_PIN_INDEX_WL_DATA_OUT  -> /WRL_DAT
        46, // CYW43_PIN_INDEX_WL_DATA_IN   -> /WRL_DAT
        46, // CYW43_PIN_INDEX_WL_HOST_WAKE -> /WRL_DAT
        45, // CYW43_PIN_INDEX_WL_CLOCK     -> /WRL_CLK
        47  // CYW43_PIN_INDEX_WL_CS        -> /WRL_~{CS}
    };
    cyw43_set_pins_wl(cyw43_pin_array);
    init_cyw43_wifi();
}
