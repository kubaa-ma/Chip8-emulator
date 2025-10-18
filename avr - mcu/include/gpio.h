/* Copyright 2024, inunix3
 *
 * This file is licensed under the MIT license. See the LICENSE.md for more information.
 */

/*
 * GPIO manipulation macros.
 */

#pragma once

#include "bit_defs.h"

#include <avr/io.h>

#define gpio_set_high(_n, _pin) set_bit(PORT##_n, P##_n##_pin)
#define gpio_set_low(_n, _pin) clear_bit(PORT##_n, P##_n##_pin)
#define gpio_toggle(_n, _pin) toggle_bit(PORT##_n, P##_n##_pin)

#define gpio_set_input(_n, _pin) clear_bit(DDR##_n, P##_n##_pin)
#define gpio_set_input_pullup(_n, _pin) gpio_set_input(_n, _pin); set_high(_n, _pin)
#define gpio_set_output(_n, _pin) set_bit(DDR##_n, P##_n##_pin)

#define gpio_read(_n, _pin) ((PIN##_n) & (1 << P##_n##_pin))
#define gpio_is_high(_n, _pin) (gpio_read_pin(_n, _pin) == 1)
#define gpio_is_low(_n, _pin) (gpio_read_pin(_n, _pin) == 0)
